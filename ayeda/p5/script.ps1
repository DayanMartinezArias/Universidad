<#
.SYNOPSIS
    Script para automatizar la creación de usuarios en Active Directory desde un archivo CSV.
.DESCRIPTION
    Crea usuarios, unidades organizativas y grupos según se especifique en el archivo CSV.
    Asigna contraseñas, directorios home y gestiona la pertenencia a grupos.
.PARAMETER CsvPath
    Ruta al archivo CSV con los datos de los usuarios.
.EXAMPLE
    .\CrearUsuariosAD.ps1 -CsvPath .\usuarios.csv
#>

param (
    [Parameter(Mandatory=$true)]
    [string]$CsvPath
)

# Función para verificar si existe una OU y crearla si no existe
function Ensure-OUExists {
    param (
        [string]$OUPath
    )
    
    try {
        $exists = Get-ADOrganizationalUnit -Filter "distinguishedName -eq '$OUPath'" -ErrorAction Stop
        if (-not $exists) {
            # Dividir la ruta DN en sus componentes
            $parts = $OUPath -split '(?<![\\]),'
            $ouName = ($parts[0] -split '=')[1]
            $parentPath = ($parts[1..($parts.Length-1)] -join ',').Trim()
            
            # Crear recursivamente las OUs padres si no existen
            if ($parentPath -notmatch '^DC=') {
                Ensure-OUExists -OUPath $parentPath
            }
            
            New-ADOrganizationalUnit -Name $ouName -Path $parentPath -ProtectedFromAccidentalDeletion $false
            Write-Host "OU creada: $OUPath" -ForegroundColor Green
        }
    }
    catch {
        Write-Host "Error al verificar/crear la OU $OUPath : $_" -ForegroundColor Red
        exit 1
    }
}

# Función para verificar si existe un grupo y crearlo si no existe
function Ensure-GroupExists {
    param (
        [string]$GroupName,
        [string]$OUPath
    )
    
    try {
        $group = Get-ADGroup -Filter {Name -eq $GroupName} -ErrorAction SilentlyContinue
        if (-not $group) {
            $group = New-ADGroup -Name $GroupName -GroupScope Global -Path $OUPath -PassThru
            Write-Host "Grupo creado: $GroupName en $OUPath" -ForegroundColor Green
        }
        return $group
    }
    catch {
        Write-Host "Error al verificar/crear el grupo $GroupName : $_" -ForegroundColor Red
        exit 1
    }
}

# Función para crear el directorio home del usuario
function Create-HomeDirectory {
    param (
        [string]$Username,
        [string]$HomeDrive,
        [string]$HomePath
    )
    
    try {
        $homeFolder = Join-Path -Path "\\fileserver\Personal\Casa" -ChildPath $Username
        if (-not (Test-Path $homeFolder)) {
            New-Item -Path $homeFolder -ItemType Directory | Out-Null
            $acl = Get-Acl $homeFolder
            $user = Get-ADUser $Username
            $userSid = $user.SID
            
            # Configurar permisos
            $accessRule = New-Object System.Security.AccessControl.FileSystemAccessRule(
                $userSid,
                "Modify",
                "ContainerInherit,ObjectInherit",
                "None",
                "Allow"
            )
            $acl.AddAccessRule($accessRule)
            Set-Acl -Path $homeFolder -AclObject $acl
            Write-Host "Directorio home creado: $homeFolder" -ForegroundColor Green
        }
        return $homeFolder
    }
    catch {
        Write-Host "Error al crear el directorio home para $Username : $_" -ForegroundColor Red
    }
}

# Verificar si el módulo de ActiveDirectory está disponible
if (-not (Get-Module -Name ActiveDirectory -ErrorAction SilentlyContinue)) {
    try {
        Import-Module ActiveDirectory -ErrorAction Stop
    }
    catch {
        Write-Host "No se pudo cargar el modulo ActiveDirectory. Asegurese de que las herramientas de RSAT estan instaladas." -ForegroundColor Red
        exit 1
    }
}

# Verificar si el archivo CSV existe
if (-not (Test-Path $CsvPath)) {
    Write-Host "El archivo CSV no existe en la ruta especificada: $CsvPath" -ForegroundColor Red
    exit 1
}

# Importar datos del CSV
$users = Import-Csv -Path $CsvPath -Header "nombre_usuario", "unidad_organizativa", "grupos" -Delimiter ","

# Procesar cada usuario
foreach ($user in $users) {
    $username = $user.nombre_usuario.Trim()
    $ou = $user.unidad_organizativa.Trim()
    $groups = $user.grupos -split ',' | ForEach-Object { $_.Trim() } | Where-Object { $_ -ne "" }
    
    # Verificar si el usuario ya existe
    if (Get-ADUser -Filter { SamAccountName -eq $username } -ErrorAction SilentlyContinue) {
        Write-Host "El usuario $username ya existe en el dominio. Omitiendo..." -ForegroundColor Yellow
        continue
    }
    
    # Verificar/Crear la OU
    try {
        $ouDN = "OU=$ou," + (Get-ADDomain).DistinguishedName
        Ensure-OUExists -OUPath $ouDN
    }
    catch {
        Write-Host "Error al procesar la OU para $username : $_" -ForegroundColor Red
        continue
    }
    
    # Verificar/Crear los grupos
    $groupsToAdd = @()
    foreach ($group in $groups) {
        if ($group) {
            $adGroup = Ensure-GroupExists -GroupName $group -OUPath $ouDN
            $groupsToAdd += $adGroup.DistinguishedName
        }
    }
    
    # Crear el usuario
    try {
        $password = ConvertTo-SecureString -String $username -AsPlainText -Force
        
        $userParams = @{
            SamAccountName        = $username
            Name                 = $username
            GivenName            = $username.Split('.')[0]
            Surname             = $username.Split('.')[-1]
            DisplayName          = $username
            UserPrincipalName    = "$username@$((Get-ADDomain).DNSRoot)"
            Path                 = $ouDN
            AccountPassword      = $password
            Enabled              = $true
            ChangePasswordAtLogon = $false
            PasswordNeverExpires = $true
            HomeDrive            = "N:"
            HomeDirectory        = "\\fileserver\Personal\Casa\$username"
        }
        
        New-ADUser @userParams
        Write-Host "Usuario creado: $username" -ForegroundColor Green
        
        # Crear directorio home
        Create-HomeDirectory -Username $username -HomeDrive "N:" -HomePath "\\fileserver\Personal\Casa\$username"
        
        # Agregar usuario a los grupos
        foreach ($groupDN in $groupsToAdd) {
            Add-ADGroupMember -Identity $groupDN -Members $username
            Write-Host "Usuario $username anadido al grupo $($groupDN -replace '^CN=([^,]+).*$','$1')" -ForegroundColor Cyan
        }
    }
    catch {
        Write-Host "Error al crear el usuario $username : $_" -ForegroundColor Red
    }
}

Write-Host "Proceso completado." -ForegroundColor Green

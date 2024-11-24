`timescale 1 ns / 10 ps
module microc_tb;
// declaración de variables
//...
reg clk, reset, s_inc, s_inm, we3, wez; 
reg [2:0] Op; 
wire [5:0] Opcode;
wire z;

// instanciación del camino de datos
//...
microc mic_1(Opcode, z, clk, reset, s_inc, s_inm, we3, wez, Op); 

// generación de reloj clk
//...

initial clk = 1;
always #10 clk = ~clk;

integer i;


// Reseteo y configuración de salidas del testbench
initial
begin
  $dumpfile("microc_tb.vcd");
  for (i = 0; i < 15; i = i + 1) begin
    $dumpvars(0,mic_1.regs.R[i]);
  end
  $dumpvars;
  // ... señal de reset
  reset = 1;
  #5
  reset = 0;
end

// Bloque simulación señales control por ciclo

initial
begin
    #10
  // retardos y señales para ejecutar primera instrucción (ciclo 1)
    s_inc = 1'b1;
    s_inm = 1'b1;
    we3 = 1'b1;
    wez = 1'b0;
    Op = 3'b000; // Establece un valor para Op (por ejemplo, 000
    #20
  // retardos y señales para ejecutar segunda instrucción (ciclo 2)
    s_inc = 1'b1;
    s_inm = 1'b1;
    we3 = 1'b1;
    wez = 1'b0;
    Op = 3'b000; // Establece un valor para Op (por ejemplo, 000
    #20
    // retardos y señales para ejecutar segunda instrucción (ciclo 3)
    s_inc = 1'b1;
    s_inm = 1'b0;
    we3 = 1'b1;
    wez = 1'b1;
    Op = 3'b011; // Establece un valor para Op (por ejemplo, 000
    #20
    s_inc = 1'b0;
    s_inm = 1'b0;
    we3 = 1'b0;
    wez = 1'b0;
    Op = 3'b000
    #20
    s_inc = 1'b0;
    s_inm = 1'b0;
    we3 = 1'b0;
    wez = 1'b0;
    Op = 3'b000
    #20
    s_inc = 1'b1;
    s_inm = 1'b1;
    we3 = 1'b1;
    wez = 1'b0;
    Op = 3'b000; // Establece un valor para Op (por ejemplo, 000
    #10
  // ...
  $finish;
end

endmodule

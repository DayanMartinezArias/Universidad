data <- c(1, 2, 3, 4, 4, 5, 6, 7, 3, 4, 3, 2, 3.4, 1, 1, 4, 5, -33 , 6, 7)

# Imprime la tabla de frecuencia
freq_table <- table(data)
print(freq_table)

# Imprime la frecuencia relativa
rel_freq <- prop.table(freq_table)
print(rel_freq)

# imprime la media
mean_value <- mean(data)
print(mean_value)

# imprime la mediana
median_value <- median(data)
print(median_value)

varianze <- var(data)
print(varianze)

desv <- sd(data)
print(desv)

minim <- min(data)
print(minim)

curve(2*x + 4*x^2 / 3*x^3, 1, 10, col = "green")


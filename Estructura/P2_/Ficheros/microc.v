module microc(output wire [5:0] Opcode, output wire z, input wire clk, reset, s_inc, s_inm, we3, wez, input wire [2:0] Op);
//Microcontrolador sin memoria de datos de un solo ciclo

wire [9:0] salida_pc;
wire [9:0] entrada_pc

// mux
wire [10:0] salida_mux;
wire dir_salto;
mux2 #(10) mux(salida_mux, salida_memoria[9:0], salida_sumador, s_inc);

// sumador + 1
wire [10:0] salida_sumador;
sum sumador (salida_sumador, 10'b0000000001, salida_pc);

// mem
wire [15:0] salida_memoria;
memprog memor(salida_memoria, clk, salida_pc);

wire [8:0] RD_1;
wire [8:0] RD_2;
regfile regs(RD_1, RD_2, clk, we3, salida_memoria[11:8], salida_memoria[7:4], salida_memoria[3:0], WD3);

wire [8:0] WD3;
mux2 #(8) seg_mux(WD3, salida_alu, salida_memoria[11:4]);

wire zero;
wire [8:0] salida_alu;
alu alu1(salida_alu, zero, RD_1, RD_2, op);

ffd zer(clk, reset, d, )

endmodule

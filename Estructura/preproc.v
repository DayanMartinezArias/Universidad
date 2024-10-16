module preprocess(output wire [3:0] AMod, output wire [3:0] BMod, input wire [3:0] A, input wire [3:0] B, input wire [2:0] Op);
  // Señales de control
  wire add1, cpl, op1_A, op2_B;
  
  // Asignaciones de las señales de control
  assign add1 = Op[0] | Op[1] & ~Op[0];
  assign cpl = ~Op[2] & ~Op[1] & Op[0]; 
  assign op1_A = (Op[1] & ~Op[0]) | (Op[2] & ~Op[1]);
  assign op2_B = (Op[1] & ~Op[0]) | (Op[2] & ~Op[1]);

  // Multiplexores
  wire [3:0] add_out, op2_B_out;
  mux2_4 mux_add(add_out, 4'b0000, 4'b0001, add1); 
  mux2_4 mux_op1_A(AMod, add_out, A, op1_A);
  mux2_4 mux_op2_B(op2_B_out, A, B, op2_B);

  // Complementador a 1
  compl1 compl(BMod, op2_B_out, cpl); 
endmodule

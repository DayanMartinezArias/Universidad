module alu(output wire [3:0] R, output wire zero, carry, sign, input wire [3:0] A, B, input wire c_in, input wire [2:0] Op); 
  
  // preprocesador
  wire [3:0] A_mod, B_mod;
  preprocess prep(A_mod, B_mod, A, B, Op);
  
  // sumador
  wire[3:0] adder_sum;
  sum4 add(adder_sum, carry, A_mod, B_mod, c_in);

  // unidad lógica
  wire [3:0] ul_out;
  ul4 ul(ul_out, B_mod, A_mod, Op[1:0]);

  // mux
  mux2_4 mux(R, adder_sum, ul_out, Op[2]);
  
  // Flags
  assign sign = R[3];
  assign zero = ~R[0] & ~R[1] & ~R[2] & ~R[3];

endmodule
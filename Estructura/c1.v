module cl(output wire out, input wire a, b, input wire [1:0] S);
  wire and_res, or_res, xor_res, not_res;

  assign and_res = a & b; 
  assign or_res  = a | b; 
  assign xor_res = a ^ b;  
  assign not_res = ~a;     

  mux4_1 mux4_1_v1(out, and_res, or_res, xor_res, not_res, S);
endmodule


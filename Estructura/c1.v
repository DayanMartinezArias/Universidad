module cl(output wire out, input wire a, b, input wire [1:0] S);
  wire and_res, or_res, xor_res, not_res;

  assign and_res = a & b;   // AND operation
  assign or_res  = a | b;   // OR operation
  assign xor_res = a ^ b;   // XOR operation
  assign not_res = ~a;      // NOT operation (only 'a', as per the problem)

  mux4_1 mux4_1_v1(out, and_res, or_res, xor_res, not_res, S);

endmodule
module sum4(output wire [3:0] S, output wire c_out, input wire [3:0] A, input wire [3:0] B, input wire c_in); 
  wire out_fa1, out_fa2, out_fa3;

  adder fa1(out_fa1, S[0], A[0], B[0], c_in);
  adder fa2(out_fa2, S[1], A[1], B[1], out_fa1);
  adder fa3(out_fa3, S[2], A[2], B[2], out_fa2);
  adder fa4(c_out, S[3], A[3], B[3], out_fa3);
endmodule
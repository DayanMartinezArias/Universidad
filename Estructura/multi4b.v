module mux2_4(output wire [3:0] Out, input wire [3:0] A, input wire [3:0] B, input wire s);
  wire not_s;
  
  not not1(not_s, s);
  
  assign Out[0] = (A[0] & not_s) | (B[0] & s);
  assign Out[1] = (A[1] & not_s) | (B[1] & s);
  assign Out[2] = (A[2] & not_s) | (B[2] & s);
  assign Out[3] = (A[3] & not_s) | (B[3] & s);

endmodule

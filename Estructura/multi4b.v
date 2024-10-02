module mux2_4(output wire [3:0] Out, input wire [3:0] A, input wire [3:0] B, input wire s);

wire not_s;
not not1(not_s, s);
assign Out = (A & {4{not_s}}) | (B & {4{s}});

endmodule
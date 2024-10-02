module fa(output wire c_out, output wire sum, input wire a, input wire b, input wire c_in);

wire sum1;
wire carry1;
wire carry2;

ha ha1(sum1, carry1, a, b);
ha ha2(sum, carry2, sum1, c_in);

or or1(c_out, carry2, carry1);

endmodule
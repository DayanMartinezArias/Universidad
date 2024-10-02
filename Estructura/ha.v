module ha (output wire suma, output wire carry, input wire a, input wire b);
  xor xor1(suma, a, b);
  and and1(carry, a, b);
endmodule

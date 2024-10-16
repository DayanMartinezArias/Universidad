module compl1(output wire [3:0] Out, input wire [3:0] Inp, input wire cpl);
 // mux2_4 mux_cpl(Out, ~Inp, Inp, cpl);
  assign Out = cpl ? ~Inp : Inp;
endmodule
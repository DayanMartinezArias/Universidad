`timescale 1 ns / 10ps

module fa_tb;

reg test_a, test_b, test_c_in;
wire test_suma, test_carry;

fa sumador1(test_carry, test_suma, test_a, test_b, test_c_in);

initial
begin
 $monitor("tiempo=%0d a=%b b=%b c_in=%b suma=%b acarreo=%b", $time, test_a, test_b, test_c_in, test_suma, test_carry);
    // Test vector 0
    test_a = 1'b0;
    test_b = 1'b0;
    test_c_in = 1'b0;
    #20;

    // Test vector 1
    test_a = 1'b0;
    test_b = 1'b1;
    test_c_in = 1'b0;
    #20;

    // Test vector 2
    test_a = 1'b1;
    test_b = 1'b0;
    test_c_in = 1'b0;
    #20;

    // Test vector 3
    test_a = 1'b1;
    test_b = 1'b1;
    test_c_in = 1'b0;
    #20;

    // Test vector 4 (with carry-in)
    test_a = 1'b0;
    test_b = 1'b0;
    test_c_in = 1'b1;
    #20;

    // Test vector 5 (with carry-in)
    test_a = 1'b1;
    test_b = 1'b0;
    test_c_in = 1'b1;
    #20;

    // Test vector 6 (with carry-in)
    test_a = 1'b0;
    test_b = 1'b1;
    test_c_in = 1'b1;
    #20;

    // Test vector 7 (with carry-in)
    test_a = 1'b1;
    test_b = 1'b1;
    test_c_in = 1'b1;
    #20;
 
 $finish; //fin simulacion
end

initial
begin
  $dumpfile("fa.vcd");
  $dumpvars;
end

endmodule
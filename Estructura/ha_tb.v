`timescale 1 ns / 10ps

module ha_tb;

reg test_a, test_b;
wire test_suma, test_carry;

ha semisumador1(test_suma, test_carry, test_a, test_b);

initial
begin
 $monitor("tiempo=%0d a=%b b=%b suma=%b acarreo=%b", $time, test_a, test_b, test_suma, test_carry);
 //vector de test 0
 test_a = 1'b0;
 test_b = 1'b0;
 #20;
 //vector de test 1
 test_a = 1'b0;
 test_b = 1'b1;
 #20;
 //vector de test 2
 test_a = 1'b1;
 test_b = 1'b0;
 #20;
 //vector de test 3
 test_a = 1'b1;
 test_b = 1'b1;
 #20
 
 $finish; //fin simulacion
end

initial
begin
  $dumpfile("ha_v1_tb.vcd");
  $dumpvars;
end

endmodule
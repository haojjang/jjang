// #########################
// ## Author: Justin Fung ##
// #########################

module ROM_128x32 ( output reg [31:0] Q,
					input [6:0] A,
					input clk );

	reg [31:0] ROM [0:127];

	initial begin
	  $readmemb("ROM.txt", ROM);
	end

	always @*
	  Q <= ROM[ A[6:0] ];

endmodule 
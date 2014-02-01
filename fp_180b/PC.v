// #########################
// ## Author: Justin Fung ##
// #########################

module PC ( output reg signed [31:0] addr,
			input [31:0] val,
			input clk, reset, set, start, load );

	reg [31:0] count;		
			
	always @*
		if (reset)  count = -1;
		else if (set)
			count = val;
		else if (start)
		    count = count + 1;
		else if (load)
			addr = count;

endmodule 
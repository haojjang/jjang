// ###########################
// ##  Author: Justin Fung  ##
// ###########################

module hex_display ( output [0:6] HEX,
		     input [3:0] data );

	reg [6:0] HEX_temp;

	parameter BLANK = 7'b111_1111;
	parameter ZERO = 7'b000_0001;
	parameter ONE = 7'b100_1111;
	parameter TWO = 7'b001_0010;
	parameter THREE = 7'b000_0110;
	parameter FOUR = 7'b100_1100;
	parameter FIVE = 7'b010_0100;
	parameter SIX = 7'b010_0000;
	parameter SEVEN = 7'b000_1111;
	parameter EIGHT = 7'b000_0000;
	parameter NINE = 7'b000_1100;
	parameter A = 7'b000_1000;
	parameter B= 7'b110_0000;
	parameter C = 7'b011_0001;
	parameter D = 7'b100_0010;
	parameter E = 7'b011_0000;
	parameter F = 7'b011_1000;

	always @( data )
		case ( data )
			0: HEX_temp = ZERO;
			1: HEX_temp = ONE;
			2: HEX_temp = TWO;
			3: HEX_temp = THREE;
			4: HEX_temp = FOUR;
			5: HEX_temp = FIVE;
			6: HEX_temp = SIX;
			7: HEX_temp = SEVEN;
			8: HEX_temp = EIGHT;
			9: HEX_temp = NINE;
			10: HEX_temp = A;
			11: HEX_temp = B;
			12: HEX_temp = C;
			13: HEX_temp = D;
			14: HEX_temp = E;
			15: HEX_temp = F;
			default: HEX_temp = ZERO;
		endcase

	assign HEX = HEX_temp;

endmodule 
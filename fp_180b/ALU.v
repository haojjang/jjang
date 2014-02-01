// #######################
// # Author: Justin Fung #
// #######################

module ALU ( output signed [31:0] Y,
	     output Z, N,
	     input signed [31:0] A, B,
	     input add, subtract, bitAnd, bitOr, complement );
		

	assign    Y = add ? B + A :
		      subtract ? B - A :
		      bitAnd ? B & A :
		      bitOr ? B | A :
		      complement ? ~A : 0;

	assign Z = Y == 0 ? 1 : 0;
	assign N = Y < 0 ? 1 : 0;

endmodule

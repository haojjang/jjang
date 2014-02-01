// #########################
// ## Author: Justin Fung ##
// #########################

module fp_mul ( output reg [31:0] p,
		output reg done,
		input [31:0] N1, N2,
		input CLOCK_50, start, restart );

	reg [47:0] product;
	reg [1:0] CURRENT_STATE;

	parameter STATE0 = 2'b00,
		  STATE1 = 2'b01,
		  STATE2 = 2'b10;

	always @( posedge CLOCK_50 )
		if(restart)
		begin
		  CURRENT_STATE = 0;
		end
		else if(start)
		  case(CURRENT_STATE)
		    STATE0:
			begin
			  done = 0;
			  p = 0;
			  CURRENT_STATE = STATE1;
			end

		    STATE1:
			begin
		          product = {1'b1, N1[22:0]} * {1'b1, N2[22:0]};
			  CURRENT_STATE = STATE2;
			end

		    STATE2:
			begin
		  	  p[31] = N1[31] ^ N2[31];
			  if(product[47] == 1)
			  begin
			    p[30:23] = (N1[30:23] + N2[30:23]) - 127 + 1;
			    p[22:0] = product[46:24];
			  end
			  else if(product[46] == 1)
			  begin
			    p[30:23] = (N1[30:23] + N2[30:23]) - 127;
		  	    p[22:0] = product[45:23];
			  end
		  	  done = 1;
			end
		  endcase

endmodule

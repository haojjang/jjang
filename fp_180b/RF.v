// #########################
// ## Author: Justin Fung ##
// #########################

module RF ( output reg [31:0] X, Y,
			output reg done,
			input [31:0] data,
			input [4:0] Rz, Ry, Rx,
			input [5:0] opcode, 
			input set, wrt );

	wire zero;
	reg [31:0] MEM [0:31];

	assign zero = 0;
	
	always @*
	    if (set)  done = 0;
		else
		  begin
		  
			done = 0;
			MEM[0] = zero; 
			X = MEM[Rx];
			Y = MEM[Ry];
			
			if (wrt)
			  if (opcode == 10)
				begin
				  if (Rz != 0)
				    MEM[Rz][31:16] = data;
				  done = 1;
				end
			  else
			    begin
				  if (Rz != 0)
				    MEM[Rz] = data;
				  done = 1;
				end
				
			if ( (opcode == 13) || (opcode == 14) )
			  done = 1;
				  
		  end 

endmodule 
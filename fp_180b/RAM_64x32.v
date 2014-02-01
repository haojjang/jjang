// #######################
// # Author: Justin Fung #
// #######################

module RAM_64x32 ( output reg [31:0] Q,
					output reg done,
					output reg [17:0] LED_reg, 
					output reg [31:0] Seg_reg,
			  input [17:0] switches,
    		  input [31:0] Addr, D,
			  input en, wrt );

	reg [31:0] RAM[0:255];

	always @*
	  begin
		RAM[129] = { 14'b00000000000000, switches };
	    LED_reg = RAM[128];
	    Seg_reg = RAM[192];
	  
	    if (en)  done = 0;
		else
		 
		  begin
		    if (wrt)
			  begin
	            RAM[Addr] = D;
				done = 1;
			  end
			else
			  begin
			    Q = RAM[Addr];
				done = 1;
			  end
		  end
	  end

endmodule 
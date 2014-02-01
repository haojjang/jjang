// #########################
// ## Author: Justin Fung ##
// #########################

module fp_add ( output reg [31:0] sum,
				output reg done,
				input [31:0] N1, N2,
				input clk, restart, start );

	reg [32:0] float1, float2, swap;
	reg [23:0] cumsum;
	reg [7:0] d;
	reg [4:0] count;
	reg carry_out;
	reg [3:0] CURRENT_STATE;
				  
	parameter [3:0] STATE0 = 4'b0000, STATE1 = 4'b0001,
			STATE2 = 4'b0010, STATE3 = 4'b0011,
			STATE4 = 4'b0100, STATE5 = 4'b0101,
			STATE6 = 4'b0110, STATE7 = 4'b0111,
			STATE8 = 4'b1000, STATE9 = 4'b1001,
			STATE10= 4'b1010;

	
	always @( posedge clk )
		if(restart)
		begin
		  CURRENT_STATE = STATE0;
		end
		else if (start)
		begin
		  
		  case( CURRENT_STATE )
			STATE0:
				begin
				  count = 0;
				  carry_out = 0;
				  done = 0;
				  cumsum = 0;
				  swap = 0;
				  sum = 0;
			      float1[22:0] = N1[22:0];
				  float1[32:24] = N1[31:23];
				  float2[22:0] = N2[22:0];
				  float2[32:24] = N2[31:23];
			      if( (N1[30:23] == 0) || (N2[30:23] == 0) )  CURRENT_STATE = STATE1;  
			      else
				  begin
				    float1[23] = 1;
					float2[23] = 1;
				    CURRENT_STATE = STATE2;
				  end
				end
				
			STATE1:
				begin
				  if( N1[30:23] == 0 )  float1[23] = 0;
				  else  float1[23] = 1;
				  if( N2[30:23] == 0 )  float2[23] = 0;
				  else  float2[23] = 1;
				  CURRENT_STATE = STATE2;
				end
				
			STATE2:
				if( N1[30:23] < N2[30:23] )
				begin
				  swap = float2;
				  float2 = float1;
				  float1 = swap;
				  CURRENT_STATE = STATE3;
				end
				else
				  CURRENT_STATE = STATE3;

			STATE3:
				begin
				  if( float2[31:24] >= float1[31:24] )
				  begin
				    float2[23:0] = float2[23:0] >> (float2[31:24] - float1[31:24]);
				    d = (float2[31:24] - float1[31:24]);
				    CURRENT_STATE = STATE4;
				  end
				  else
				  begin
				    float2[23:0] = float2[23:0] >> (float1[31:24] - float2[31:24]);
				    d = (float1[31:24] - float2[31:24]);
				    CURRENT_STATE = STATE4;
				  end
				end
					
			STATE4:
				begin
				  float2[23:0] = float2[23:0] >> (float2[31:24] - float1[31:24]);
				  if( float1[32] != float2[32] )
				  begin
				    float2[23:0] = ~float2[23:0] + 1;
				    CURRENT_STATE = STATE5;
				  end
				  else
				    CURRENT_STATE = STATE5; 
				end
				
			STATE5:
				begin
				  {carry_out, cumsum} = float1[23:0] + float2[23:0];
				  CURRENT_STATE = STATE6;
				end

			STATE6:
				if( (float1[32] != float2[32]) && (cumsum[23] == 1) && (carry_out != 1) )
				begin
				  cumsum = ~cumsum + 1;
				  CURRENT_STATE = STATE7;
				end
				else
				  CURRENT_STATE = STATE7;
				
			STATE7:
				begin
				  if( (float1[32] == float2[32]) && (carry_out == 1) )
				    CURRENT_STATE = STATE8;
				  else
				  begin
				    count = count + 1;
				    cumsum = cumsum << 1;
					if(cumsum[23] == 1)
					begin
					  d = d - count;
					  CURRENT_STATE = STATE9;
					end
					else if(count > 24)
					begin
					  cumsum = 0;
					  d = 0;
					  CURRENT_STATE = STATE10;
					end
				  end
				end

			STATE8:
				begin
				  cumsum = (cumsum >> 1) + 24'b100000000000_000000000000;
				  d = d + 1;
				  CURRENT_STATE = STATE9;
				end
				
			STATE9:
				begin
				  if( float1[31:24] > float2[31:24] )
				    cumsum[23] = N1[31];
				  if( float2[31:24] > float1[31:24] )
				    cumsum[23] = N2[31];
					
				  if( float1[31:24] == float2[31:24] )
				  begin
				    if( float1[22:0] > float2[22:0] )
					  cumsum[23] = float1[32];
					if( float2[22:0] > float1[22:0] )
					  cumsum[23] = float2[32];
				  end
				  CURRENT_STATE = STATE10;
				end
				
			STATE10:
				begin
				  sum[31] = cumsum[23];
				  sum[30:23] = d + 127;
				  sum[22:0] = cumsum[22:0];
				  done = 1;
				end
				
		  endcase
		end
	  
endmodule

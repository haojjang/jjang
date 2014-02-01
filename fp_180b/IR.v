// #########################
// ## Author: Justin Fung ##
// #########################

module IR ( output reg [5:0] opcode,
			output reg [4:0] Rz, Ry, Rx,
			output reg [15:0] Imm,
			input [31:0] inset,
			input clk );

	parameter [5:0] ADD = 0, ADDI = 1, SUB = 2, SUBI = 3,
					AND = 4, ANDI = 5, OR = 6, ORI = 7,
					COM = 8, LDI = 9, LUI = 10, LW = 11,
					LWI = 12, SW = 13, SWI = 14, BNZ = 15,
					BPL = 16, JMP = 17, JAL = 18, JR = 19,
					FPADD = 20, FPMULT = 21;

	always @*
	  begin
	    opcode <= inset[31:26];
	    Rz <= inset[25:21];
		Ry <= inset[20:16];
		
		case ( inset[31:26] )
		  ADDI : Imm <= inset[15:0];
		  SUBI : Imm <= inset[15:0];
		  ANDI : Imm <= inset[15:0];
		  ORI : Imm <= inset[15:0];
		  LDI : Imm <= inset[15:0];
		  LUI : Imm <= inset[15:0];
		  LWI : Imm <= inset[15:0];
		  SWI : Imm <= inset[15:0];
		  BNZ : Imm <= inset[15:0];
		  BPL : Imm <= inset[15:0];
		  JMP : Imm <= inset[15:0];
		  JAL : Imm <= inset[15:0];
		  default : Rx <= inset[15:11];
		endcase
	  end

endmodule 
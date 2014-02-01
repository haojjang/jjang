// #########################
// ## Author: Justin Fung ##
// #########################

module WB_data ( output reg [31:0] data,
				input [31:0] FPadd, FPmult, ALU, WORD, Jal, Ldi, Lui,
				input [5:0] opcode,
				input enable );

	parameter [5:0] ADD = 0, ADDI = 1, SUB = 2, SUBI = 3,
					AND = 4, ANDI = 5, OR = 6, ORI = 7,
					COM = 8, LDI = 9, LUI = 10, LW = 11,
					LWI = 12, SWc = 13, SWIc = 14, BNZ = 15,
					BPL = 16, JMP = 17, JAL = 18, JR = 19,
					FPADD = 20, FPMULT = 21;			
				
	always @*
		begin
		  if (enable)
		  begin
			
		    if ( (opcode == ADD) || (opcode == ADDI) || (opcode == SUB) ||
				(opcode == SUBI) || (opcode == AND) || (opcode == ANDI) ||
				(opcode == OR) || (opcode == ORI) || (opcode == COM) )
			  data <= ALU;
			  
			if ( (opcode == LW) || (opcode == LWI) )
			  data <= WORD;
			
		    case (opcode)
			  FPADD : data <= FPadd;
			  FPMULT : data <= FPmult;
			  JAL : data <= Jal;
			  LDI : data <= Ldi;
			  LUI : data <= Lui;
		    endcase
			
		  end
		end

endmodule 
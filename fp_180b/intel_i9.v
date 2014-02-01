// #########################
// ## Author: Justin Fung ##
// #########################

module intel_i9 ( output [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0,
				  output [17:0] LEDR,
				input [17:0] SW,
				input CLOCK_50, reset );

	wire [31:0] ROM_Q, RAM_Q, XRF, YRF, data, ALU_out, sum, product;
	wire [31:0] Seg_Regs;
	wire [17:0] LED_reg;
	wire [15:0] Imm;
	wire [5:0] opcode;
	wire [4:0] Rz, Ry, Rx;
	wire RF_done, N, Z, fp_done, RAM_done;
	
	reg [3:0] CURRENT_STATE;
	reg [31:0] val, data_reg, X, Y, ALU_op, FPsum, FPprod, X_RAM, Y_RAM;
	reg signed [31:0] count;
	reg [31:0] Ldi, Lui, Jal;
	reg PCRF_start, enable, update, fp_restart, fp_start, RAM_wrt;
	reg addALU, subALU, andALU, orALU, compALU;
	
	
	parameter [5:0] ADD = 0, ADDI = 1, SUB = 2, SUBI = 3,
					AND = 4, ANDI = 5, OR = 6, ORI = 7,
					COM = 8, LDI = 9, LUI = 10, LW = 11,
					LWI = 12, SWc = 13, SWIc = 14, BNZ = 15,
					BPL = 16, JMP = 17, JAL = 18, JR = 19,
					FPADD = 20, FPMULT = 21;
	
	parameter [3:0] STATE0 = 4'b0000, STATE1 = 4'b0001,
			STATE2 = 4'b0010, STATE3 = 4'b0011,
			STATE4 = 4'b0100, STATE5 = 4'b0101,
			STATE6 = 4'b0110, STATE7 = 4'b0111,
			STATE8 = 4'b1000, STATE9 = 4'b1001,
			STATE10= 4'b1010;		
	
			
	always @( posedge CLOCK_50 )
		if (reset)
		  begin
		    CURRENT_STATE = STATE0;
			count = 0;
		  end
		else
		  begin
		  
		    case (CURRENT_STATE)
			
				STATE0 :
				  begin
					PCRF_start = 1;
					enable = 0;
					fp_restart = 1;
					fp_start = 0;
					
					CURRENT_STATE = STATE1;
				  end
				
				STATE1 :
				  begin
				    if ( (opcode == JMP) || (opcode == JAL) || (opcode == JR) ||
							((opcode == BNZ) && (Z == 0)) || 
							((opcode == BPL) && (N == 0)) ) 
					  CURRENT_STATE = STATE2;
					else if ( (opcode == FPADD || opcode == FPMULT) )
					  CURRENT_STATE = STATE3;
					else
				
				    begin
				      PCRF_start = 0;
					  enable = update;
					  if (RF_done)
					    begin
						  count = count + 1;
					      CURRENT_STATE = STATE0;
					    end
					end
				  end

				STATE2 :
				  begin
				    PCRF_start = 0;
					enable = update;
					if (RF_done)
					    begin
						  if (opcode == JAL)
						      enable = 0;
						  count = val;
					      CURRENT_STATE = STATE0;
					    end
				  end
				  
				STATE3 :
				  begin
				    fp_restart = 0;
					fp_start = 1;
				    PCRF_start = 0;
					if (fp_done)
					  CURRENT_STATE = STATE1;
				  end
				  
			endcase
			
		  end
		

	always @*
		begin
		
			if ( (opcode == ADD) || (opcode == ADDI) || (opcode == SUB) || 
				(opcode == SUBI) || (opcode == AND) || (opcode == ANDI) || 
				(opcode == OR) || (opcode == ORI) || (opcode == COM) )
			  ALU_op = ALU_out;
		
			case (opcode)
			
				ADD :
				    if (CURRENT_STATE == STATE1)
					  begin
					    X = XRF;
						Y = YRF;
				        addALU = 1;
					    subALU = 0;
					    andALU = 0;
					    orALU = 0;
					    compALU = 0;
						update = 1;
					   end
				  
				ADDI :
					if (CURRENT_STATE == STATE1)
					  begin
					    X = { 4'h0000, Imm };
						Y = YRF;
				        addALU = 1;
					    subALU = 0;
					    andALU = 0;
					    orALU = 0;
					    compALU = 0;
						update = 1;
					   end
				
				SUB :
					if (CURRENT_STATE == STATE1)
					  begin
					    X = XRF;
						Y = YRF;
				        addALU = 0;
					    subALU = 1;
					    andALU = 0;
					    orALU = 0;
					    compALU = 0;
						update = 1;
					   end
				
				SUBI :
					if (CURRENT_STATE == STATE1)
					  begin
					    X = { 4'h0000, Imm };
						Y = YRF;
				        addALU = 0;
					    subALU = 1;
					    andALU = 0;
					    orALU = 0;
					    compALU = 0;
						update = 1;
					   end
				
				AND :
					if (CURRENT_STATE == STATE1)
					  begin
					    X = XRF;
						Y = YRF;
				        addALU = 0;
					    subALU = 0;
					    andALU = 1;
					    orALU = 0;
					    compALU = 0;
						update = 1;
					   end
				
				ANDI :
					if (CURRENT_STATE == STATE1)
					  begin
					    X = { 4'h0000, Imm };
						Y = YRF;
				        addALU = 0;
					    subALU = 0;
					    andALU = 1;
					    orALU = 0;
					    compALU = 0;
						update = 1;
					   end
				
				OR :
					if (CURRENT_STATE == STATE1)
					  begin
					    X = XRF;
						Y = YRF;
				        addALU = 0;
					    subALU = 0;
					    andALU = 0;
					    orALU = 1;
					    compALU = 0;
						update = 1;
					   end
				
				ORI :
					if (CURRENT_STATE == STATE1)
					  begin
					    X = { 4'h0000, Imm };
						Y = YRF;
				        addALU = 0;
					    subALU = 0;
					    andALU = 0;
					    orALU = 1;
					    compALU = 0;
						update = 1;
					   end
				
				COM :
					if (CURRENT_STATE == STATE1)
					  begin
					    X = XRF;
						Y = YRF;
				        addALU = 0;
					    subALU = 0;
					    andALU = 0;
					    orALU = 0;
					    compALU = 1;
						update = 1;
					   end
				
				LDI : 
					if (CURRENT_STATE == STATE1)
					  begin
					    Ldi = { 4'h0000, Imm };
						update = 1;
					  end
				
				LUI :
					if (CURRENT_STATE == STATE1)
					  begin
					    Lui = { 4'h0000, Imm };
						update = 1;
					  end
				
				LW :
					if (CURRENT_STATE == STATE1)
					  begin
					    RAM_wrt = 0;
					    X_RAM = XRF;
						Y_RAM = YRF;
						update = 1;
					  end
				
				LWI :
					if (CURRENT_STATE == STATE1)
					  begin
						RAM_wrt = 0;
					    X_RAM = { 4'h0000, Imm };
						Y_RAM = YRF;
						update = 1;
					  end
				
				SWc : 
					if (CURRENT_STATE == STATE1)
					  begin
					    RAM_wrt = 1;
					    X_RAM = XRF;
						Y_RAM = YRF;
						update = 1;
					  end
				
				SWIc :
					if (CURRENT_STATE == STATE1)
					  begin
					    RAM_wrt = 1;
					    X_RAM = { 4'h0000, Imm };
						Y_RAM = YRF;
						update = 1;
					  end
				
				BNZ :
					if (CURRENT_STATE == STATE2)
					  begin
					    if ( Z == 0 )
						  val = { 4'h0000, Imm };
					  update = 1;
					  end
				
				BPL :
					if (CURRENT_STATE == STATE2)
					  begin
					    if ( N == 0 )
						  val = { 4'h0000, Imm };
					  update = 1;
					  end
				
				JMP :
					if (CURRENT_STATE == STATE2)
					  begin
						val = { 4'h0000, Imm };
						update = 1;
					  end
				
				JAL : 
					if (CURRENT_STATE == STATE2)
					  begin
						val = { 4'h0000, Imm };
						Jal = count + 1;
						update = 1;
					  end
				
				JR :
					if (CURRENT_STATE == STATE2)
					  begin
						val = XRF;
						update = 1;
					  end
				
				FPADD :
					if ( (CURRENT_STATE == STATE1) || (CURRENT_STATE == STATE2) )
					  begin
					    X = XRF;
					    Y = YRF;
						FPsum = sum;
						update = 1;
					  end

				FPMULT :
					if ( (CURRENT_STATE == STATE1) || (CURRENT_STATE == STATE2) )
					  begin
					    X = XRF;
					    Y = YRF;
						FPprod = product;
						update = 1;
					  end
		
			endcase
		
		end
	
	
	assign LEDR = LED_reg;
	
	
//	PC PC1( .addr(count), .clk(CLOCK_50), .reset(reset), .set(set),
//			.start(PCRF_start), .val(val), .load(load) );
	
	ROM_128x32 ROM1( .Q(ROM_Q), .A( count[6:0] ), .clk(CLOCK_50) );
	
	IR IR1( .opcode(opcode), .Rz(Rz), .Ry(Ry), .Rx(Rx), .Imm(Imm),
			.inset(ROM_Q), .clk(CLOCK_50) );
	
	RF RF1( .X(XRF), .Y(YRF), .data(data), .Rz(Rz), .Ry(Ry), .Rx(Rx),
			.opcode(opcode), .done(RF_done), .set(PCRF_start),
			.wrt(enable) );

    WB_data WB1( .data(data), .ALU(ALU_op), .FPadd(FPsum), .FPmult(FPprod),
				 .Ldi(Ldi), .Lui(Lui), .Jal(Jal), .WORD(RAM_Q),
				 .opcode(opcode), .enable(enable) );
			
	ALU ALU1( .Y( ALU_out ), .Z( Z ), .N( N ), .A(X), .B(Y), .add(addALU), 
			.subtract(subALU), .bitAnd(andALU), .bitOr(orALU),
			.complement(compALU) );
	
	RAM_64x32 RAM1( .Q(RAM_Q), .Addr(X_RAM), .D(Y_RAM), .en(PCRF_start), .wrt(RAM_wrt),
				.done(RAM_done), .LED_reg(LED_reg), .Seg_reg(Seg_Regs),
				.switches(SW) );
	
	fp_add add1( .sum(sum), .done(fp_done), .N1(X), .N2(Y), .clk(CLOCK_50),
					.restart(fp_restart), .start(fp_start) );
					
	fp_mul mul1( .p(product), .done(fp_done), .N1(X), .N2(Y), .CLOCK_50(CLOCK_50),
					.restart(fp_restart), .start(fp_start) );
					
					
	hex_display H0( .HEX(HEX0), .data(Seg_Regs[3:0]) );
	hex_display H1( .HEX(HEX1), .data(Seg_Regs[7:4]) );
	hex_display H2( .HEX(HEX2), .data(Seg_Regs[11:8]) );
	hex_display H3( .HEX(HEX3), .data(Seg_Regs[15:12]) );
	hex_display H4( .HEX(HEX4), .data(Seg_Regs[19:16]) );
	hex_display H5( .HEX(HEX5), .data(Seg_Regs[23:20]) );
	hex_display H6( .HEX(HEX6), .data(Seg_Regs[27:24]) );
	hex_display H7( .HEX(HEX7), .data(Seg_Regs[31:28]) );

endmodule 
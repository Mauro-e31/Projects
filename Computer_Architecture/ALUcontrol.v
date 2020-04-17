`timescale 1ns/1ps
/* Declaring the opcode of our ALU operations to avoid re-writing them .
*/
`define AND 11'b10001010000
`define OR  11'b10101010000
`define ADD 11'b10001011000
`define SUB 11'b11001011000

module ALUControl (ALUCtrl, ALUop,Opcode);
/* declaring the inputs and output ports.
*/
input [1:0 ] ALUop;
input [10:0] Opcode;
output reg [3:0] ALUCtrl;
/*  Here we use our ALUop and Opcode to decide on our operations
*/
	always @ (ALUop or Opcode) begin
	case(ALUop)
	2'b00: begin
	/*This case is for our LDUR and STUR where our opcode doesnt matter.
	*/
	ALUCtrl <= #2 4'b0010;
	end

	2'b01: begin
	/* This case is for our condition branch zero, where our opcode doesnt matter either.
	*/
	ALUCtrl <= #2 4'b0111;
	end

	2'b10: begin
	/*Here we have a different case statements, where we have to take the *
	* opcode into consideration.
	*/
	case(Opcode)
	`ADD: begin
	ALUCtrl <= #2 4'b0010;
	end
	`SUB: begin
	ALUCtrl <= #2 4'b0110;
	end
	`AND: begin
	ALUCtrl <= #2 4'b0000;
	end
	`OR: begin
	ALUCtrl <= #2 4'b0001;
	end
	endcase//////

	end

        endcase
    end
endmodule

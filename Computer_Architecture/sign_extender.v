`timescale 1ns/1ps

module SignExtender(BusImm,Imm32,Ctrl);
output wire [63:0] BusImm;
input wire [31:0] Imm32;
input wire [1:0] Ctrl; /// controls which instructions we are using.
/// Crl[1] == Uncondbranch signal
// Ctrl[0]== branch signal

wire extBit;
wire [25:0] offset; //offset for B instruction
wire [18:0] boffset; // ofset for CBZ instruction
wire [8:0] ldoffset;  // ofset for LDUR and STUR
wire [63:0] tempWire;  // temporary storage.


assign offset = {Imm32[25:0]}; // B offset comes from first 26 bits of instruction.
assign boffset = {Imm32[23:5]}; // CBZ offset comes from the 19 bits of instructions.
assign ldoffset = {Imm32[20:12]}; // LDUR and STUR comes from 9 bits of instructions.

/* if Uncondbranch is 1, and branch is 0, we sign extend the first 26 bits of the instructions
 * else it means we are using a conditional branch. and sign extend the 19 bits from instructions.
 * this gets store in a tempWire.
*/
assign #2 tempWire= ((Ctrl[1]==1) && (Ctrl[0]==0) )? ({{38{offset[25]}},offset}) : ({{45{boffset[18]}},boffset});

/* however, if we are not using neither conditional or Uncondbranch instructions,
 * it must mean we are using a LDUR or STUR, so we sign extend the 9 bits from the isntructions
 * We make this be our output. Else, we dont change anything and let our BusImm be our tempWire.
*/

assign #2 BusImm = ((Ctrl[1]==0) && (Ctrl[0]==0) )? ({{55{ldoffset[8]}},ldoffset}): tempWire;

// NOTE* we are not shifting the instructions here, the shifting happens in our NextPClogic module.
endmodule

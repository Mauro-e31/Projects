`timescale 1ns / 1ps

module NextPClogic (NextPC, CurrentPC,SignExtImm64,Branch,ALUZero,Uncondbranch);
/* declaring our inputs and outputs
*/
  input  [63:0] CurrentPC,SignExtImm64;
  input  Branch,ALUZero,Uncondbranch;
  output reg [63:0] NextPC;


  //assign SE = SignExtImm64;
  always @ ( *) begin

   if((Branch==1) && (ALUZero==1)) begin
   NextPC <=  #3 (SignExtImm64<<2'b10) + (CurrentPC);
   /*This means that if our branch is 1 and zero is , then we will branch according to the *
   SignExtImm64 + current PC. Else it fails and we just add 4 to our current PC.
   */
   end
////////////////////
   else if (Uncondbranch) begin
   NextPC <= #3 ((SignExtImm64<<2'b10) + (CurrentPC));
   /* this is simmmilar to our conditional branch, except here we have the ability to branch *
   to a further destination .
   */
   end
//////////////////////////
   else  begin
   NextPC <= #3 (CurrentPC + 3'b100);
   /*This is our default routine for instructions, we update our pc by 4.
   */
 end
end

// assign NextPC = CurrentPC + 4'b100;

endmodule // NextPClogic

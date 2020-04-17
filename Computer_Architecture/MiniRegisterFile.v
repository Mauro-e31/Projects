`timescale 1ns / 1ps

module RegisterFile(BusA, BusB, BusW, RA,RB,RW, RegWr, Clk);
    output [63:0] BusA;
    output [63:0] BusB;
    input [63:0] BusW;
    input [4:0] RA;
    input [4:0] RB;
    input [4:0] RW;
    input RegWr;
    input Clk;
    reg[63:0] registers [31:0];

////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

    //registers[5'd31] = #3 0;
    initial begin
    registers[31]=0;
    end

    assign #2 BusA = registers[RA];
    assign #2 BusB = registers[RB];

    always @ (negedge Clk) begin
        if(RegWr)
            if(RW==31) begin
            registers[31]<= #3 0;
            end
            else begin
            registers[RW] <= #3 BusW;
            end
    end
endmodule

`timescale 1ns / 1ps

module register_n_bit #(parameter n = 8)(
    input [n-1:0] data_input,
    input [1:0] fun_sel,
    input e, clk, 
    output [n-1:0] data_output
);

reg [n-1:0] data;

always @ (posedge clk) begin
    if (e) begin
        case (fun_sel)
            2'b00: data <= 0;   // reset
            2'b01: data <= data_input;  // load
            2'b10: data <= data - 1;    // decrement
            2'b11: data <= data + 1;    // increment
            default: data <= data;  // default
        endcase
     end
end
assign data_output = data;
endmodule

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

module IR(
    input [7:0] data_input,
    input [1:0] fun_sel,
    input e, clk, lh,
    output [15:0] data_output
    );
    wire [15:0] in_16; 
    assign in_16[7:0] = (lh == 0)? data_input : data_output[7:0];
    assign in_16[15:8] = (lh == 0)? data_output[15:8] : data_input;
    register_n_bit #(16) ir_reg(in_16, fun_sel, e, clk, data_output);
endmodule

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

module Memory(
    input wire[7:0] address,
    input wire[7:0] data,
    input wire wr, //Read = 0, Write = 1
    input wire cs, //Chip is enable when cs = 0
    input wire clock,
    output reg[7:0] o // Output
);
    //Declaration o?f the RAM Area
    reg[7:0] RAM_DATA[0:255];
    //Read Ram data from the file
    initial $readmemh("RAM.mem", RAM_DATA);
    //Read the selected data from RAM
    always @(*) begin
        o = ~wr && ~cs ? RAM_DATA[address] : 8'hZ;
    end
    
    //Write the data to RAM
    always @(posedge clock) begin
        if (wr && ~cs) begin
            RAM_DATA[address] <= data; 
        end
    end
endmodule

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

module RF(
    input [7:0] in,
    input [2:0] O1Sel,
    input [2:0] O2Sel,
    input [1:0] fun_sel,
    input [3:0] RSel,
    input [3:0] TSel,
    input clk,
    output reg [7:0] O1,O2
    );   
		
     wire [7:0] t1,t2,t3,t4,r1,r2,r3,r4; //register outputs
    
     register_n_bit #(8) T1(in, fun_sel, TSel[3], clk, t1); //these are the T registers, 
     register_n_bit #(8) T2(in, fun_sel, TSel[2], clk, t2); //e.g when the 4 bit binary Tsel value's first bit is 1, 
     register_n_bit #(8) T3(in, fun_sel, TSel[1], clk, t3); //only first T register is enabled.
     register_n_bit #(8) T4(in, fun_sel, TSel[0], clk, t4);
     
     register_n_bit #(8) R1(in, fun_sel, RSel[3], clk, r1); //these are the R registers, 
     register_n_bit #(8) R2(in, fun_sel, RSel[2], clk, r2); //e.g when the 4 bit binary Rsel value's first bit is 1, 
     register_n_bit #(8) R3(in, fun_sel, RSel[1], clk, r3); //only first R register is enabled.
     register_n_bit #(8) R4(in, fun_sel, RSel[0], clk, r4);
	 
		 initial O1 = 0;
		 initial O2 = 0;
		 
         always @(*) begin             
             case (O1Sel)
                 3'b000 : O1 = t1;
                 3'b001 : O1 = t2;
                 3'b010 : O1 = t3;
                 3'b011 : O1 = t4;
                 3'b100 : O1 = r1;
                 3'b101 : O1 = r2;
                 3'b110 : O1 = r3;
                 3'b111 : O1 = r4;
             endcase
             case (O2Sel)
                 3'b000 : O2 = t1;
                 3'b001 : O2 = t2;
                 3'b010 : O2 = t3;
                 3'b011 : O2 = t4;
                 3'b100 : O2 = r1;
                 3'b101 : O2 = r2;
                 3'b110 : O2 = r3;
                 3'b111 : O2 = r4;
             endcase
         end
endmodule


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

module ARF(
    input[7:0]in,  //8 bit input
    input [1:0]OutASel,
    input [1:0]OutBSel,
    input [1:0]fun_sel, //Function Selector
    input [3:0]RSel,
    input clk,
    output reg [7:0]OutA,OutB //8 bit outputs
    );
    wire [7:0]out1,out2,out3,out4;
        
        register_n_bit #(8) AR(in,fun_sel,RSel[3],clk,out1);
        register_n_bit #(8) SP(in,fun_sel,RSel[2],clk,out2);
        register_n_bit #(8) PCPast(in,fun_sel,RSel[1],clk,out3);
        register_n_bit #(8) PC(in,fun_sel,RSel[0],clk,out4);
        
         always @(*) begin             
            case (OutASel)
                2'b00 : OutA = out1;
                2'b01 : OutA = out2;
                3'b10 : OutA = out3;
                3'b11 : OutA = out4;               
           endcase
           case (OutBSel)
                3'b00 : OutB = out1;
                3'b01 : OutB = out2;
                3'b10 : OutB = out3;
                3'b11 : OutB = out4;
          endcase
          end
endmodule

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

module ALU(
    input[7:0]A,B,  //8 bit inputs
    input [3:0]fun_sel, //Function Selector
    output reg [3:0]flag,  //(zero,carry,negative,overflow)
    output reg [7:0]ALUOut //8 bit output
    );
    reg [8:0]tmp;
    reg signA, signB;
    
    initial begin
    flag = 4'b0000; ALUOut = 0;
    end
    
    always@(*)
    begin
        case(fun_sel)      
        4'b0000: ALUOut = A; //A   
        
        4'b0001: ALUOut = B; //B
        
        4'b0010: ALUOut = ~A;// A not
        
        4'b0011: ALUOut = ~B;// B not

        4'b0100:           // A + B
        begin
            tmp = A + B + flag[2];
            flag[2] = tmp[8];
            ALUOut = tmp[7:0];
            flag[0] = (A[7]==0&&B[7]==0&&tmp[7])||(A[7]==1&&B[7]==1&&~tmp[7]);         
        end
        
        4'b0101:    // A -B
        begin
            tmp = A - B;
            flag[2] = tmp[8];
            ALUOut = tmp[7:0];
            flag[0] = (A[7]==0&&B[7]==1&&tmp[7])||(A[7]==1&&B[7]==0&&~tmp[7]);
        end
        
        4'b0110:   //Compare A and B
        begin
            signA = A[7];
            signB = B[7];
            tmp = A - B;
            flag[2] = tmp[8];
            flag[0] = (A[7]==0&&B[7]==1&&tmp[7])||(A[7]==1&&B[7]==0&&~tmp[7]);
            ALUOut = tmp>0 ? A : 0;
        end

        4'b0111: ALUOut = A & B;  //A&B

        4'b1000: ALUOut = A | B;  //A|B

        4'b1001: ALUOut = ~(A & B);  //A NAND B

        4'b1010: ALUOut = A & ~B | ~A & B;   //A XOR B

        4'b1011: 
        begin  //LSL A
            flag[2] = A[7];
            ALUOut = A << 1;
        end
           
        4'b1100:  //LSR A
        begin
            flag[2] = A[0];
            ALUOut = A >> 1;
        end
        
        4'b1101:  //ASL A
        begin
            flag[0] = A[7] ^ A[6];
            ALUOut = A <<< 1;
        end
        
        4'b1110:  //ASR A
        begin
            ALUOut = {A[7],A[7:1]};
        end
        
        4'b1111:  //CSR A
        begin
        flag[2] = A[0];
        flag[0] = A[7] ^ A[6];
        ALUOut = {A[0],A[7:1]};
        end        
    endcase
    flag[3] = (ALUOut == 0)? 1'b1 : 1'b0;
    if(fun_sel != 4'b1101)
    flag[1] = (ALUOut[7] == 1)? 1'b1 : 1'b0;
    end
endmodule


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

module ALU_System(
    input [2:0] RF_OutASel, 
    input [2:0] RF_OutBSel, 
    input [1:0] RF_FunSel,
    input [3:0] RF_RSel, RF_TSel,
    input [3:0] ALU_FunSel,
    input [1:0] ARF_OutCSel, 
    input [1:0] ARF_OutDSel, 
    input [1:0] ARF_FunSel,
    input [3:0] ARF_RegSel,
    input IR_LH,
    input IR_Enable,
    input [1:0] IR_Funsel,
    input Mem_WR,
    input Mem_CS,
    input [1:0] MuxASel,
    input [1:0] MuxBSel,
    input MuxCSel,
    input Clock
);
    reg [7:0] MuxAOut;
    reg [7:0] MuxBOut;
    reg [7:0] MuxCOut;
    wire [7:0] AOut;
    wire [7:0] BOut;
    wire [7:0] ARF_AOut;
    wire [7:0] Address;
    wire [7:0] ALUOut;
    wire [3:0] ALUOutFlag;
    wire [7:0] MemoryOut;
    wire [15:0] IROut;
    
    initial begin
    MuxAOut = 0;
    MuxBOut = 0;
    MuxCOut = 0;
    end
    
always @(*) begin
    case (MuxASel) //mux A
    2'b00: MuxAOut <= ALUOut;
    2'b01: MuxAOut <= MemoryOut;
    2'b10: MuxAOut <= IROut[7:0];
    2'b11: MuxAOut <= ARF_AOut;
    endcase
    
    case (MuxBSel) //mux B
    2'b00: MuxBOut <= ALUOut;
    2'b01: MuxBOut <= MemoryOut;
    2'b10: MuxBOut <= IROut[7:0];
    2'b11: MuxBOut <= ARF_AOut;
    endcase
       
    case(MuxCSel) //mux C
    1'b0: MuxCOut <= AOut;
    1'b1: MuxCOut <= ARF_AOut;
    endcase
end
		
    RF rf(MuxAOut, RF_OutASel, RF_OutBSel, RF_FunSel, RF_RSel, RF_TSel, Clock, AOut, BOut);
    ALU alu(MuxCOut, BOut, ALU_FunSel, ALUOutFlag, ALUOut);
    ARF arf(MuxBOut, ARF_OutCSel, ARF_OutDSel, ARF_FunSel, ARF_RegSel, Clock, ARF_AOut, Address);
    Memory memory(Address, ALUOut, Mem_WR, Mem_CS, Clock, MemoryOut);
    IR ir(MemoryOut, IR_Funsel, IR_Enable, Clock, IR_LH, IROut);
    
endmodule

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

module ControlUnit
(
    input Clock
);
    reg[2:0] RF_OutASel; 
    reg[2:0] RF_OutBSel; 
    reg[1:0] RF_FunSel;
    reg[3:0] RF_RSel, RF_TSel;
    reg[3:0] ALU_FunSel;
    reg[1:0] ARF_OutCSel; 
    reg[1:0] ARF_OutDSel; 
    reg[1:0] ARF_FunSel;
    reg[3:0] ARF_RegSel;
    reg      IR_LH;
    reg      IR_Enable;
    reg[1:0]      IR_Funsel;
    reg      Mem_WR;
    reg      Mem_CS;
    reg[1:0] MuxASel;
    reg[1:0] MuxBSel;
    reg MuxCSel;
    reg SeqCtr_Reset;
    wire [2:0] SeqCtr_T;
    
    
    SequentialCounter seqCounter(Clock, SeqCtr_Reset, SeqCtr_T);
    
    ALU_System aluSystem(
    .RF_OutASel(RF_OutASel), 
    .RF_OutBSel(RF_OutBSel), 
    .RF_FunSel(RF_FunSel),
    .RF_RSel(RF_RSel),
    .RF_TSel(RF_TSel),
    .ALU_FunSel(ALU_FunSel),
    .ARF_OutCSel(ARF_OutCSel), 
    .ARF_OutDSel(ARF_OutDSel), 
    .ARF_FunSel(ARF_FunSel),
    .ARF_RegSel(ARF_RegSel),
    .IR_LH(IR_LH),
    .IR_Enable(IR_Enable),
    .IR_Funsel(IR_Funsel),
    .Mem_WR(Mem_WR),
    .Mem_CS(Mem_CS),
    .MuxASel(MuxASel),
    .MuxBSel(MuxBSel),
    .MuxCSel(MuxCSel),
    .Clock(Clock));
    
    parameter FETCH1 = 0, FETCH2 = 1, DECODE = 2, OPERATION = 3;
    parameter AND = 0, OR = 1, NOT = 2, ADD = 3, SUB = 4, LSR = 5, LSL = 6, INC = 7, DEC = 8, BRA = 9, BNE = 10, MOV = 11, LD = 12, ST = 13, PUL = 14, PSH = 15;

    
    reg [3:0] opcode;
    wire z;
    assign z = aluSystem.ALUOutFlag[2];
    reg addressingMode;
    reg [1:0] RSel;
    reg [7:0] address;
    
    reg [3:0] dstReg;
    reg [3:0] SReg1;
    reg [3:0] SReg2;
    reg [1:0] ARFValue;
    reg [1:0] RFValue;
    
    always @(posedge Clock)
    begin
        case (SeqCtr_T)
            FETCH1: 
            begin
                ARF_OutDSel = 2'b11;
                ARF_FunSel = 2'b11;
                ARF_RegSel = 4'b0011;
                Mem_CS = 1;
                Mem_WR = 0;
                IR_Enable = 1;
                IR_Funsel = 2'b01;
                IR_LH = 0;
                RF_RSel = 4'b0000;
                RF_FunSel = 2'b00;
                SeqCtr_Reset = 0;
            end
            FETCH2:
            begin
                ARF_OutDSel = 2'b11;
                ARF_FunSel = 2'b11;
                ARF_RegSel = 4'b0011;
                Mem_CS = 1;
                Mem_WR = 0;
                IR_Enable = 1;
                IR_Funsel = 2'b01;
                IR_LH = 1;
                RF_RSel = 4'b0000;
                RF_FunSel = 2'b00;
                SeqCtr_Reset = 0;
            end

            DECODE:
            begin
                opcode = ALU_System.IROut[15:12];
                addressingMode = ALU_System.IROut[10];
                RSel = ALU_System.IROut[9:8];
                address = ALU_System.IROut[7:0];
                dstReg = ALU_System.IROut[11:8];
                SReg1 = ALU_System.IROut[7:4];
                SReg2 = ALU_System.IROut[3:0];
            end

            OPERATION:
            begin
                case (opcode)
                AND:
                    begin
                        if (dstReg[2] == 0) //rf to rf
                        begin
                            if(SReg1[2] == 0 && SReg2[2] == 0) // rf rf rf
                            begin
                                RF_OutASel = {1'b1, SReg1[1:0]};
                                RF_OutBSel = {1'b1, SReg2[1:0]};
                                RF_FunSel = 2'b01;
                                RF_RSel = dstReg[1:0];
                                ALU_FunSel = 4'b0111;
                                Mem_CS = 1;
                                Mem_WR = 0;
                                MuxASel = 2'b00; 
                                MuxCSel = 0;
                            end
                            else
                            begin
                                if(SReg1[2] != 0 && SReg2[2] == 0) //arf rf rf
                                begin
                                    ARFValue = SReg1[1:0];
                                    RFValue = SReg2[1:0];   
                                end
                                else if (SReg1[2] == 0 && SReg2[2] != 0) //rf arf rf
                                begin
                                    ARFValue = SReg2[1:0];
                                    RFValue = SReg1[1:0];                 
                                end
                                else
                                begin
                                    ARFValue = 0;
                                    RFValue = 0;
                                end
                                RF_OutBSel = {1'b1, RFValue};
                                ARF_OutCSel = ARFValue;
                                RF_FunSel = 2'b01;
                                RF_RSel = dstReg[1:0];
                                ALU_FunSel = 4'b0111;
                                Mem_CS = 1;
                                Mem_WR = 0;
                                MuxASel = 2'b00;
                                MuxCSel = 1;
                            end
                                 
                        end
                        else //rf to arf
                            if(SReg1[2] == 0 && SReg2[2] == 0) // rf rf arf
                        begin
                            RF_OutASel = {1'b1, SReg1[1:0]};
                            RF_OutBSel = {1'b1, SReg2[1:0]};
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            ALU_FunSel = 4'b0111;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 0;
                        end
                        else
                        begin
                            if(SReg1[2] != 0 && SReg2[2] == 0) //arf rf arf
                            begin
                                ARFValue = SReg1[1:0];
                                RFValue = SReg2[1:0];   
                            end
                            else if (SReg1[2] == 0 && SReg2[2] != 0) //rf arf arf
                            begin
                                ARFValue = SReg2[1:0];
                                RFValue = SReg1[1:0];                 
                            end
                            else
                            begin
                                ARFValue = 0;
                                RFValue = 0;
                            end
                            RF_OutBSel = {1'b1, RFValue};
                            ARF_OutCSel = ARFValue;
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            ALU_FunSel = 4'b0111;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 1;
                        end
                    end

                    OR:
                    begin
                        if (dstReg[2] == 0) //rf to rf
                        begin
                            if(SReg1[2] == 0 && SReg2[2] == 0) // rf rf rf
                            begin
                                RF_OutASel = {1'b1, SReg1[1:0]};
                                RF_OutBSel = {1'b1, SReg2[1:0]};
                                RF_FunSel = 2'b01;
                                RF_RSel = dstReg[1:0];
                                ALU_FunSel = 4'b1000;
                                Mem_CS = 1;
                                Mem_WR = 0;
                                MuxASel = 2'b00;
                                MuxCSel = 0;
                            end
                            else
                            begin
                                if(SReg1[2] != 0 && SReg2[2] == 0) //arf rf rf
                                begin
                                    ARFValue = SReg1[1:0];
                                    RFValue = SReg2[1:0];   
                                end
                                else if (SReg1[2] == 0 && SReg2[2] != 0) //rf arf rf
                                begin
                                    ARFValue = SReg2[1:0];
                                    RFValue = SReg1[1:0];                 
                                end
                                else
                                begin
                                    ARFValue = 0;
                                    RFValue = 0;
                                end
                                RF_OutBSel = {1'b1, RFValue};
                                ARF_OutCSel = ARFValue;
                                RF_FunSel = 2'b01;
                                RF_RSel = dstReg[1:0];
                                ALU_FunSel = 4'b1000;
                                Mem_CS = 1;
                                Mem_WR = 0;
                                MuxASel = 2'b00;
                                MuxCSel = 1;
                            end
                                 
                        end
                        else //rf to arf
                            if(SReg1[2] == 0 && SReg2[2] == 0) // rf rf arf
                        begin
                            RF_OutASel = {1'b1, SReg1[1:0]};
                            RF_OutBSel = {1'b1, SReg2[1:0]};
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            ALU_FunSel = 4'b1000;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 0;
                        end
                        else
                        begin
                            if(SReg1[2] != 0 && SReg2[2] == 0) //arf rf arf
                            begin
                                ARFValue = SReg1[1:0];
                                RFValue = SReg2[1:0];   
                            end
                            else if (SReg1[2] == 0 && SReg2[2] != 0) //rf arf arf
                            begin
                                ARFValue = SReg2[1:0];
                                RFValue = SReg1[1:0];                 
                            end
                            else
                            begin
                                ARFValue = 0;
                                RFValue = 0;
                            end
                            RF_OutBSel = {1'b1, RFValue};
                            ARF_OutCSel = ARFValue;
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            ALU_FunSel = 4'b1000;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 1;
                        end
                    end

                    NOT:
                    begin
                        if (dstReg[2] == 0 && SReg1[2] == 0) //rf to rf
                        begin
                            RF_OutBSel = {1'b1, SReg1[1:0]};
                            RF_FunSel = 2'b01;
                            RF_RSel = dstReg[1:0];
                            ALU_FunSel = 4'b0011;
                            Mem_CS = 1;
                            Mem_WR = 0;       
                            MuxASel = 2'b00;                     
                        end
                        if (dstReg[2] == 0 && SReg1[2] != 0) //arf to rf
                        begin
                            RF_FunSel = 2'b01;
                            RF_RSel = dstReg[1:0];
                            ALU_FunSel = 4'b0010;
                            ARF_OutCSel = SReg1[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;       
                            MuxASel = 2'b00; 
                            MuxCSel = 1;
                        end
                        if (dstReg[2] != 0 && SReg1[2] == 0) //rf to arf
                        begin
                            RF_OutBSel = {1'b1, SReg1[1:0]};
                            ALU_FunSel = 4'b0011;
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00; 
                        end
                        if (dstReg[2] != 0 && SReg1[2] != 0) //arf to arf
                        begin
                            ALU_FunSel = 4'b0010;
                            ARF_OutCSel = SReg1[1:0];
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 1;
                        end
                    end

                    ADD:
                    begin
                        if (dstReg[2] == 0) //rf to rf
                        begin
                            if(SReg1[2] == 0 && SReg2[2] == 0) // rf rf rf
                            begin
                                RF_OutASel = {1'b1, SReg1[1:0]};
                                RF_OutBSel = {1'b1, SReg2[1:0]};
                                RF_FunSel = 2'b01;
                                RF_RSel = dstReg[1:0];
                                ALU_FunSel = 4'b0100;
                                Mem_CS = 1;
                                Mem_WR = 0;
                                MuxASel = 2'b00;
                                MuxCSel = 0;
                            end
                            else
                            begin
                                if(SReg1[2] != 0 && SReg2[2] == 0) //arf rf rf
                                begin
                                    ARFValue = SReg1[1:0];
                                    RFValue = SReg2[1:0];   
                                end
                                else if (SReg1[2] == 0 && SReg2[2] != 0) //rf arf rf
                                begin
                                    ARFValue = SReg2[1:0];
                                    RFValue = SReg1[1:0];                 
                                end
                                else
                                begin
                                    ARFValue = 0;
                                    RFValue = 0;
                                end
                                RF_OutBSel = {1'b1, RFValue};
                                ARF_OutCSel = ARFValue;
                                RF_FunSel = 2'b01;
                                RF_RSel = dstReg[1:0];
                                ALU_FunSel = 4'b0100;
                                Mem_CS = 1;
                                Mem_WR = 0;
                                MuxASel = 2'b00;
                                MuxCSel = 1;
                            end
                                 
                        end
                        else //rf to arf
                            if(SReg1[2] == 0 && SReg2[2] == 0) // rf rf arf
                        begin
                            RF_OutASel = {1'b1, SReg1[1:0]};
                            RF_OutBSel = {1'b1, SReg2[1:0]};
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            ALU_FunSel = 4'b0100;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 0;
                        end
                        else
                        begin
                            if(SReg1[2] != 0 && SReg2[2] == 0) //arf rf arf
                            begin
                                ARFValue = SReg1[1:0];
                                RFValue = SReg2[1:0];   
                            end
                            else if (SReg1[2] == 0 && SReg2[2] != 0) //rf arf arf
                            begin
                                ARFValue = SReg2[1:0];
                                RFValue = SReg1[1:0];                 
                            end
                            else
                            begin
                                ARFValue = 0;
                                RFValue = 0;
                            end
                            RF_OutBSel = {1'b1, RFValue};
                            ARF_OutCSel = ARFValue;
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            ALU_FunSel = 4'b0100;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 1;
                        end
                    end

                    SUB:
                    begin
                        if (dstReg[2] == 0) //rf to rf
                        begin
                            if(SReg1[2] == 0 && SReg2[2] == 0) // rf rf rf
                            begin
                                RF_OutASel = {1'b1, SReg1[1:0]};
                                RF_OutBSel = {1'b1, SReg2[1:0]};
                                RF_FunSel = 2'b01;
                                RF_RSel = dstReg[1:0];
                                ALU_FunSel = 4'b0101;
                                Mem_CS = 1;
                                Mem_WR = 0;
                                MuxASel = 2'b00;
                                MuxCSel = 0;
                            end
                            else
                            begin
                                if(SReg1[2] != 0 && SReg2[2] == 0) //arf rf rf
                                begin
                                    ARFValue = SReg1[1:0];
                                    RFValue = SReg2[1:0];   
                                end
                                else if (SReg1[2] == 0 && SReg2[2] != 0) //rf arf rf
                                begin
                                    ARFValue = SReg2[1:0];
                                    RFValue = SReg1[1:0];                 
                                end
                                else
                                begin
                                    ARFValue = 0;
                                    RFValue = 0;
                                end
                                RF_OutBSel = {1'b1, RFValue};
                                ARF_OutCSel = ARFValue;
                                RF_FunSel = 2'b01;
                                RF_RSel = dstReg[1:0];
                                ALU_FunSel = 4'b0101;
                                Mem_CS = 1;
                                Mem_WR = 0;
                                MuxASel = 2'b00;
                                MuxCSel = 1;
                            end
                                 
                        end
                        else //rf to arf
                            if(SReg1[2] == 0 && SReg2[2] == 0) // rf rf arf
                        begin
                            RF_OutASel = {1'b1, SReg1[1:0]};
                            RF_OutBSel = {1'b1, SReg2[1:0]};
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            ALU_FunSel = 4'b0101;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 0;
                        end
                        else
                        begin
                            if(SReg1[2] != 0 && SReg2[2] == 0) //arf rf arf
                            begin
                                ARFValue = SReg1[1:0];
                                RFValue = SReg2[1:0];   
                            end
                            else if (SReg1[2] == 0 && SReg2[2] != 0) //rf arf arf
                            begin
                                ARFValue = SReg2[1:0];
                                RFValue = SReg1[1:0];                 
                            end
                            else
                            begin
                                ARFValue = 0;
                                RFValue = 0;
                            end
                            RF_OutBSel = {1'b1, RFValue};
                            ARF_OutCSel = ARFValue;
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            ALU_FunSel = 4'b0101;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 1;
                        end
                    end

                    LSR:
                    begin
                        if (dstReg[2] == 0 && SReg1[2] == 0) //rf to rf
                        begin
                            RF_OutBSel = {1'b1, SReg1[1:0]};
                            RF_FunSel = 2'b01;
                            RF_RSel = dstReg[1:0];
                            ALU_FunSel = 4'b1100;
                            Mem_CS = 1;
                            Mem_WR = 0;       
                            MuxASel = 2'b00;                     
                        end
                        if (dstReg[2] == 0 && SReg1[2] != 0) //arf to rf
                        begin
                            RF_FunSel = 2'b01;
                            RF_RSel = dstReg[1:0];
                            ALU_FunSel = 4'b1100;
                            ARF_OutCSel = SReg1[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;       
                            MuxASel = 2'b00; 
                            MuxCSel = 1;
                        end
                        if (dstReg[2] != 0 && SReg1[2] == 0) //rf to arf
                        begin
                            RF_OutBSel = {1'b1, SReg1[1:0]};
                            ALU_FunSel = 4'b1100;
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00; 
                        end
                        if (dstReg[2] != 0 && SReg1[2] != 0) //arf to arf
                        begin
                            ALU_FunSel = 4'b1100;
                            ARF_OutCSel = SReg1[1:0];
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 1;
                        end
                    end

                    LSL:
                    begin
                        if (dstReg[2] == 0 && SReg1[2] == 0) //rf to rf
                        begin
                            RF_OutBSel = {1'b1, SReg1[1:0]};
                            RF_FunSel = 2'b01;
                            RF_RSel = dstReg[1:0];
                            ALU_FunSel = 4'b1011;
                            Mem_CS = 1;
                            Mem_WR = 0;       
                            MuxASel = 2'b00;                     
                        end
                        if (dstReg[2] == 0 && SReg1[2] != 0) //arf to rf
                        begin
                            RF_FunSel = 2'b01;
                            RF_RSel = dstReg[1:0];
                            ALU_FunSel = 4'b1011;
                            ARF_OutCSel = SReg1[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;       
                            MuxASel = 2'b00; 
                            MuxCSel = 1;
                        end
                        if (dstReg[2] != 0 && SReg1[2] == 0) //rf to arf
                        begin
                            RF_OutBSel = {1'b1, SReg1[1:0]};
                            ALU_FunSel = 4'b1011;
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00; 
                        end
                        if (dstReg[2] != 0 && SReg1[2] != 0) //arf to arf
                        begin
                            ALU_FunSel = 4'b1011;
                            ARF_OutCSel = SReg1[1:0];
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = dstReg[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 1;
                        end
                    end

                    INC:
                    begin
                        RF_FunSel = 2'b01;
                        RF_RSel = SReg1[1:0];
                        Mem_CS = 1;
                        Mem_WR = 0;
                    end

                    DEC:
                    begin
                        ARF_FunSel = 2'b01;
                        ARF_RegSel = SReg1[1:0];
                        Mem_CS = 1;
                        Mem_WR = 0;
                    end

                    BRA:
                    begin
                        ARF_FunSel = 2'b01;
                        ARF_RegSel = 4'b0011;
                        Mem_CS = 1;
                        Mem_WR = 0;
                        MuxBSel = 2'b10;
                    end

                    BNE:
                    begin
                        if(z)
                        begin
                            ARF_FunSel = 2'b01;
                            ARF_RegSel = 4'b0011;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b10; 
                        end
                    end

                    MOV:
                    begin
                        if (dstReg[2] == 0 && SReg1[2] == 0) //rf to rf
                        begin
                            RF_OutBSel = {1'b1, SReg1[1:0]};
                            RF_FunSel = 2'b01;
                            RF_RSel = SReg1[1:0] & dstReg[1:0];
                            ALU_FunSel = 4'b0001;
                            Mem_CS = 1;
                            Mem_WR = 0;       
                            MuxASel = 2'b00;                     
                        end
                        if (dstReg[2] == 0 && SReg1[2] != 0) //arf to rf
                        begin
                            RF_FunSel = 2'b01;
                            RF_RSel = dstReg[1:0];
                            ALU_FunSel = 4'b0001;
                            ARF_OutCSel = SReg1[1:0];
                            Mem_CS = 1;
                            Mem_WR = 0;       
                            MuxASel = 2'b11; 
                        end
                        if (dstReg[2] != 0 && SReg1[2] == 0) //rf to arf
                        begin
                            RF_OutBSel = {1'b1, SReg1[1:0]};
                            ALU_FunSel = 4'b0001;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00; 
                        end
                        if (dstReg[2] != 0 && SReg1[2] != 0) //arf to arf
                        begin
                            ALU_FunSel = 4'b0001;
                            Mem_CS = 1;
                            Mem_WR = 0;
                            MuxBSel = 2'b00;
                            MuxCSel = 1;
                        end
                    end

                    LD:
                    begin
                        ARF_FunSel = 2'b01;
                        ARF_RegSel = RSel;
                        Mem_CS = 1;
                        Mem_WR = 0;
                        if (addressingMode == 1)
                            MuxASel = 2'b10;
                        else
                            MuxASel = 2'b01;
                    end

                    ST:
                    begin
                        RF_OutBSel = {1'b1, RSel};
                        ALU_FunSel = 4'b0001;
                        Mem_CS = 0;
                        Mem_WR = 1;
                    end


                    PUL:
                    begin
                        RF_OutBSel = {1'b1, RSel};
                        ALU_FunSel = 4'b0001;
                        ARF_OutDSel = 2'b01;
                        ARF_FunSel = 2'b11;
                        ARF_RegSel = 4'b0011;
                        Mem_CS = 0;
                        Mem_WR = 1;
                    end

                    PSH:
                    begin
                        RF_OutBSel = {1'b1, RSel};
                        ALU_FunSel = 4'b0001;
                        ARF_OutDSel = 2'b01;
                        ARF_FunSel = 2'b10;
                        ARF_RegSel = 4'b0011;
                        Mem_CS = 0;
                        Mem_WR = 1;
                    end
                endcase
                SeqCtr_Reset = 1;  
            end

    endcase
        
    end

endmodule

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

module SequentialCounter
(
    input clock,
    input reset,
    output reg [3:0] T
);
initial T = 0;
    always @(posedge clock)
    begin
        if(reset)
            T <= 0;
        else
            T <= T + 1;
    end
endmodule
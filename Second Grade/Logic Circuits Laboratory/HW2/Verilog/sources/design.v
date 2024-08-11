`timescale 1ns / 1ps

module nand_gate2(input A, input B, output O);
   assign O = ~(A&B);
endmodule

module SR_latch(input S, input R, output Q, output Q_neg);
    wire NS, NR;
    assign NS = ~S;
    assign NR = ~R;
    nand_gate2 ng3(NS, Q_neg, Q);
    nand_gate2 ng4(NR, Q, Q_neg);
endmodule

module SR_latch_E(input S, input R, input E, output Q, output Q_neg);
     wire SE, RE;
     nand_gate2 ng1(S, E, SE);
     nand_gate2 ng2(R, E, RE);
     nand_gate2 ng3(SE, Q_neg, Q);
     nand_gate2 ng4(RE, Q, Q_neg);
endmodule

module SR_flipflop(input S, input R, input CLK, output Q, output Q_neg);
    wire SC, RC;
     nand_gate2 ng1(S, CLK, SC);
     nand_gate2 ng2(R, CLK, RC);
     nand_gate2 ng3(SC, Q_neg, Q);
     nand_gate2 ng4(RC, Q, Q_neg);
endmodule

module D_latch_E(input D, input E, output Q, output Q_neg);
    wire ND, NDE, DE, out3, out4;
    assign ND = ~D;
    nand_gate2 ng1(D, E, DE);
    nand_gate2 ng2(ND, E, NDE);
    nand_gate2 ng3(DE, Q_neg, out3);
    nand_gate2 ng4(NDE, Q, out4);
    assign Q = out3;
    assign Q_neg = out4;
endmodule

 module NE_D_flipflop(input D, input CLK, output Q, output Q_neg);
    wire NCLK, firstD, empty;
    assign NCLK = ~CLK;
    D_latch_E l1(D, CLK, firstD, empty);
    D_latch_E l2(firstD, NCLK, Q, Q_neg);
 endmodule

 module PE_D_flipflop(input D, input CLK, output Q, output Q_neg);
    wire NCLK, firstD, empty;
    assign NCLK = ~CLK;
    D_latch_E d1(D, NCLK, firstD, empty);
    D_latch_E d2(firstD, CLK, Q, Q_neg);
 endmodule

module PE_JK_flipflop(input J, input K, input CLK, output Q, output Q_neg);
      wire NK, KQ, JQ, D;
      nand_gate2 ng1(J, Q_neg, JQ);
      nand_gate2 ng2(K, K, NK);
      nand_gate2 ng3(NK, Q, KQ);
      nand_gate2 ng4(JQ, KQ, D);
      PE_D_flipflop ff(D, CLK, Q, Q_neg);
      
endmodule
  
module async_up_counter(input J,input K,input CLK,output[3:0] Q, output[3:0]Q_neg );
       wire in1, in2, in3, in4, w1, w2, w3, w4, w5, w6, w7;
       wire nreset, in;
       
       nand_gate2 ng1(J, K, nreset);
       nand_gate2 ng2(nreset, nreset,in);
       PE_JK_flipflop ff1(J, K, CLK, Q[0], Q_neg[0]);
       
       nand_gate2 ng3(nreset, CLK, w1);
       nand_gate2 ng4(in, Q_neg[0], w2);
       nand_gate2 ng5(w1, w2, in1);
       PE_JK_flipflop ff2(J, K, in1, Q[1],Q_neg[1]);
       
       nand_gate2 ng6(nreset, CLK, w3);
       nand_gate2 ng7(in, Q_neg[1], w4);
       nand_gate2 ng8(w3, w4, in2);
       PE_JK_flipflop ff3(J, K, in2, Q[2], Q_neg[2]);
       
       nand_gate2 ng9(nreset, CLK, w5);
       nand_gate2 ng10(in, Q_neg[2], w6);
       nand_gate2 ng11(w5, w6, in3);
       PE_JK_flipflop ff4(J, K, in3, Q[3], Q_neg[3]);   
endmodule
   
module sync_up_counter(input J, input K, input CLK, output[3:0]Q, output[3:0]Q_neg);
       wire w1, w2;
       PE_JK_flipflop ff1(J, K, CLK, Q[0], Q_neg[0]);
       
       PE_JK_flipflop ff2(Q[0], Q[0], CLK, Q[1] ,Q_neg[1]);
       assign w1 = Q[0] & Q[1];
       
       PE_JK_flipflop ff3(w1, w1, CLK, Q[2] ,Q_neg[2]);
       assign w2 = w1 & Q[2];
       
       PE_JK_flipflop ff4(w2, w2, CLK, Q[3], Q_neg[3]);
endmodule
   
module pulse_generator(input [15:0] in_data, input load, input CLK, output reg Q);
        reg [15:0] shift_register;
        
        always @(posedge CLK) begin
            if(load) begin
                Q = 1'b0;
                shift_register <= in_data;
            end
            else begin
                shift_register <= {shift_register[14:0], shift_register[15]};
            end
                assign Q =  shift_register[15];
        end
endmodule
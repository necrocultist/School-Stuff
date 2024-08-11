`timescale 1ns / 1ps

module and_gate(input A, B, output Z);
    assign Z = A & B;
endmodule

module and_gate3(input A, B, C, output Z);
    assign Z = A & B & C;
endmodule

module or_gate(input A, B, output Z);
    assign Z = A | B;
endmodule

module or_gate3(input A, B, C, output Z);
    assign Z = A | B | C;
endmodule

module nand_gate(input A, B, output Z);
    assign Z = ~(A & B);
endmodule

module nand_gate3(input A, B, C, output Z);
    assign Z = ~(A & B & C);
endmodule

module nor_gate(input A, B, output Z);
    assign Z = ~(A | B);
endmodule

module xor_gate(input A, B, output Z);
    wire NA, NB, O0, O1;
    not_gate na(A,NA);
    not_gate nb(B,NB);
    and_gate a1(A, NB, O0);
    and_gate a2(NA, B, O1);    
    or_gate o1(O0, O1, Z);
endmodule

module not_gate(input A, output Z);
    assign Z = ~ A;
endmodule

module mux_2x1(input A, B, S, output Z);
    assign Z = S ? B : A; 
endmodule

module mux_4x1(input A, B, C, D, S0, S1, output Z);
    wire O1, O2;
    mux_2x1 m1 (A, B, S0, O1);
    mux_2x1 m2 (C, D, S0, O2);
    mux_2x1 m3 (O1, O2, S1, Z);
endmodule

module mux_8x1(input A, B, C, D, E, F, G, H, S0, S1, S2, output Z);
    wire O1, O2;
    mux_4x1 m1(A, B, C, D, S0, S1, O1);
    mux_4x1 m2(E, F, G, H, S0, S1, O2);
    mux_2x1 m3(O1, O2, S2, Z);
endmodule

module dec_3x8(input A, B, C, output O0, O1, O2, O3, O4, O5, O6, O7);
    wire NA, NB, NC;
    not_gate n0(A, NA); not_gate n1(B, NB); not_gate n2(C, NC);
    and_gate3 a0(NA, NB, NC, O0);
    and_gate3 a1(A, NB, NC, O1);
    and_gate3 a2(NA, B, NC, O2);
    and_gate3 a3(A, B, NC, O3);
    and_gate3 a4(NA, NB, C, O4);        
    and_gate3 a5(A, NB, C, O5);    
    and_gate3 a6(NA, B, C, O6);    
    and_gate3 a7(A, B, C, O7);
endmodule

module part2(input A, B, C, D, output Z);
    wire NA, ND, AG1, AG2, AG3, OG1;
    not_gate na(A, NA); not_gate nd(D, ND);
    and_gate3 a0(NA, B, C, AG1);
    and_gate3 a1(A, C, D, AG2);
    and_gate a2(NA, ND, AG3);
    or_gate3 a3(AG1, AG2, AG3, Z);
endmodule

module part3(input A, B, C, D, output Z);
    wire NA, NB, ND, nabc, nabc1, nabc2, nacd, nacd1, nacd2, nbd, nbd1, nbd2;
    nand_gate na(A, A, NA);
    nand_gate nb(B, B, NB);
    nand_gate nd(D, D, ND);
    
    nand_gate3 ngabc(NA, B, C, nabc);
    nand_gate ngabc1(nabc, nabc, nabc1);
    nand_gate ngabc2(nabc1, nabc1, nabc2);
    
    nand_gate3 ngacd(A, C, D, nacd);
    nand_gate ngacd1(nacd, nacd, nacd1);
    nand_gate ngacd2(nacd1, nacd1 ,nacd2);
    
    nand_gate ngbd(NB, ND, nbd);
    nand_gate ngbd1(nbd, nbd, nbd1);
    nand_gate ngbd2(nbd1, nbd1, nbd2);
    
    nand_gate3 nf(nabc2, nacd2, nbd2, Z);
endmodule

module part4(input A, B, C, D, output Output);
    wire ND;
    not_gate n(D, ND);
    mux_8x1 mux(ND, ND, 0, 1, ND, 1, 0, D, C, B, A, Output);
endmodule

module part5_1(input A, B, C, output Z);
    wire d0, d1, d2, d3, d4, d5, d6, d7;
    dec_3x8 dec(C, B, A, d0 , d1, d2, d3, d4, d5, d6, d7);
    or_gate og1(d3, d5, Z);
endmodule

module part5_2(input A, B, C, output Z);
    wire d0, d1, d2, d3, d4, d5, d6, d7;
    dec_3x8 dec(C, B, A, d0 , d1, d2, d3, d4, d5, d6, d7);
    or_gate og1(d6 , d7, Z);
endmodule

module half_adder1(input A, B, output S, C);
    xor_gate xg1(A, B, S);
    and_gate ag1(A, B, C);
endmodule

module full_adder1(input A, B, Cin, output S, Cout);
    wire S0, C0, C1;
    half_adder1 ha(A, B, S0, C0);
    half_adder1 ha1(S0, Cin, S, C1);
    or_gate og(C0, C1, Cout);
endmodule

module full_adder4(input [3:0] A, [3:0] B, Cin, output [3:0] S, Cout);
    wire C0, C1, C2;
    full_adder1 fa0(A[0], B[0], Cin, S[0], C0);
    full_adder1 fa1(A[1], B[1], C0, S[1], C1);
    full_adder1 fa2(A[2], B[2], C1, S[2], C2);
    full_adder1 fa3(A[3], B[3], C2, S[3], Cout);
endmodule

module full_adder8(input [7:0] A, [7:0] B, input Cin, output [7:0] S, Co);
    wire [7:0] Cout;
    full_adder1 fa0(A[0], B[0], Cin, S[0], Cout[0]);
    full_adder1 fa1(A[1], B[1], Cout[0],S[1], Cout[1]);
    full_adder1 fa2(A[2], B[2], Cout[1], S[2],Cout[2]);
    full_adder1 fa3(A[3], B[3], Cout[2], S[3], Cout[3]);
    full_adder1 fa4(A[4], B[4], Cout[3], S[4], Cout[4]);
    full_adder1 fa5(A[5], B[5], Cout[4], S[5], Cout[5]);
    full_adder1 fa6(A[6], B[6], Cout[5], S[6], Cout[6]);
    full_adder1 fa7(A[7], B[7], Cout[6], S[7], Co);
endmodule

module full_adder16(input [15:0] A, [15:0] B, M, output [15:0] S, Co);
    wire C1, C2;
    wire[15:0] Bo;
    xor_gate xg0(B[0], M, Bo[0]);
    xor_gate xg1(B[1], M, Bo[1]);
    xor_gate xg2(B[2], M, Bo[2]);
    xor_gate xg3(B[3], M, Bo[3]);
    xor_gate xg4(B[4], M, Bo[4]);
    xor_gate xg5(B[5], M, Bo[5]);
    xor_gate xg6(B[6], M, Bo[6]);
    xor_gate xg7(B[7], M, Bo[7]);
    xor_gate xg8(B[8], M, Bo[8]);
    xor_gate xg9(B[9], M, Bo[9]);
    xor_gate xg10(B[10], M, Bo[10]);
    xor_gate xg11(B[11], M, Bo[11]);
    xor_gate xg12(B[12], M, Bo[12]);
    xor_gate xg13(B[13], M, Bo[13]);
    xor_gate xg14(B[14], M, Bo[14]);
    xor_gate xg15(B[15], M, Bo[15]);
    full_adder8 fa0(A[7:0], Bo[7:0], M, S[7:0], C1); 
    full_adder8 fa1(A[15:8], Bo[15:8], C1, S[15:8], Co); 
endmodule

module part11(input [15:0] A, [15:0] B, output [15:0]S, Cout);
    wire [15:0] A2X; wire c;
    full_adder16 add(A,A,0,A2X,c);
    full_adder16 sub(B, A2X, 1, S, Cout);
endmodule
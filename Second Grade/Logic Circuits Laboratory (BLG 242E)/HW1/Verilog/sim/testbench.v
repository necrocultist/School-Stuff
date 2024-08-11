`timescale 1ns / 1ps

module and_gate_test( );
    reg A;
    reg B;
    wire Z;
    and_gate uut(A, B, Z);
    initial begin
    A = 0; B = 0; #250;
    A = 1; B = 0; #250;
    A = 1; B = 1; #250;
    A = 0; B = 1; #250;
    end
endmodule

module or_gate_test();
    reg A;
    reg B;
    wire Z;
    or_gate uut(A, B, Z);
    initial begin
    A = 0; B = 0; #250;
    A = 1; B = 0; #250;
    A = 1; B = 1; #250;
    A = 0; B = 1; #250;
    end
endmodule

module not_gate_test();
    reg A;
    wire Z;
    not_gate uut(A, Z);
    initial begin
    A = 0; #250;
    A = 1; #250;
    end
endmodule

module xor_gate_test();
    reg A; 
    reg B;
    wire Z;
    xor_gate uut(A, B, Z);
    initial begin 
    A = 0; B = 0; #250;
    A = 1; B = 0; #250;
    A = 1; B = 1; #250;
    A = 0; B = 1; #250;
    end
endmodule

module nand_gate_test();
    reg A; 
    reg B;
    wire Z;
    nand_gate uut(A, B, Z);
    initial begin 
    A = 0; B = 0; #250;
    A = 1; B = 0; #250;
    A = 1; B = 1; #250;
    A = 0; B = 1; #250;
    end
endmodule

module mux_8x1_test();
    reg A; reg B; reg C; reg D; reg E; reg F; reg G; reg H; reg S0; reg S1; reg S2; wire Z;
    mux_8x1 uut(A ,B, C, D ,E, F ,G, H ,S0, S1 ,S2, Z);
    initial begin
    S0=0; S1=0; S2=0; A=0; B=0; C=0; D=0; E=0; F=0; G=0; H=0; #250;
    S0=0; S1=0; S2=0; A=1; #50;
    S0=1; S1=0; S2=0; B=0; #50;
    S0=1; S1=0; S2=0; B=1; #50;
    S0=0; S1=1; S2=0; C=0; #50;
    S0=0; S1=1; S2=0; C=1; #50;
    S0=1; S1=1; S2=0; D=0; #50;
    S0=1; S1=1; S2=0; D=1; #50;
    S0=0; S1=0; S2=1; E=0; #50;
    S0=0; S1=0; S2=1; E=1; #50;
    S0=1; S1=0; S2=1; F=0; #50;
    S0=1; S1=0; S2=1; F=1; #50;
    S0=0; S1=1; S2=1; G=0; #50;
    S0=0; S1=1; S2=1; G=1; #50;
    S0=1; S1=1; S2=1; H=0; #50;
    S0=1; S1=1; S2=1; H=1; #50;
    
    end
endmodule

module dec_3x8_test();
    reg S0; reg S1; reg S2; wire O0; wire O1; wire O2; wire O3; wire O4; wire O5; wire O6; wire O7;
    dec_3x8 uut(S0, S1, S2, O0, O1, O2, O3, O4, O5, O6, O7);
    initial begin
        S0=0; S1=0; S2=0; #100;
        S0=1; S1=0; S2=0; #100;
        S0=0; S1=1; S2=0; #100;  
        S0=1; S1=1; S2=0; #100;
        S0=0; S1=0; S2=1; #100;
        S0=1; S1=0; S2=1; #100;
        S0=0; S1=1; S2=1; #100;
        S0=1; S1=1; S2=1; #100;
    end
endmodule

module part_2_test();
    reg A; reg B; reg C; reg D; wire Z;
    part2 uut(A, B, C, D, Z); 
    initial begin
    A=0; B=0; C=0; D=0; #50;
    A=0; B=0; C=0; D=1; #50;
    A=0; B=0; C=1; D=0; #50;
    A=0; B=0; C=1; D=1; #50;
    A=0; B=1; C=0; D=0; #50;
    A=0; B=1; C=0; D=1; #50;
    A=0; B=1; C=1; D=0; #50;
    A=0; B=1; C=1; D=1; #50;
    A=1; B=0; C=0; D=0; #50;
    A=1; B=0; C=0; D=1; #50;
    A=1; B=0; C=1; D=0; #50;
    A=1; B=0; C=1; D=1; #50;
    A=1; B=1; C=0; D=0; #50;
    A=1; B=1; C=0; D=1; #50;
    A=1; B=1; C=1; D=0; #50;
    A=1; B=1; C=1; D=1; #50;
    end
endmodule

module part_3_test();
    reg A; reg B; reg C; reg D; wire Z;
    part3 uut(A, B, C, D, Z); 
    initial begin
    A=0; B=0; C=0; D=0; #50;
    A=0; B=0; C=0; D=1; #50;
    A=0; B=0; C=1; D=0; #50;
    A=0; B=0; C=1; D=1; #50;
    A=0; B=1; C=0; D=0; #50;
    A=0; B=1; C=0; D=1; #50;
    A=0; B=1; C=1; D=0; #50;
    A=0; B=1; C=1; D=1; #50;
    A=1; B=0; C=0; D=0; #50;
    A=1; B=0; C=0; D=1; #50;
    A=1; B=0; C=1; D=0; #50;
    A=1; B=0; C=1; D=1; #50;
    A=1; B=1; C=0; D=0; #50;
    A=1; B=1; C=0; D=1; #50;
    A=1; B=1; C=1; D=0; #50;
    A=1; B=1; C=1; D=1; #50;
    end
endmodule

module part_4_test();
    reg A; reg B; reg C; reg D; wire Z;
    part4 uut(A, B, C, D, Z); 
    initial begin
    A=0; B=0; C=0; D=0; #50;
    A=0; B=0; C=0; D=1; #50;
    A=0; B=0; C=1; D=0; #50;
    A=0; B=0; C=1; D=1; #50;
    A=0; B=1; C=0; D=0; #50;
    A=0; B=1; C=0; D=1; #50;
    A=0; B=1; C=1; D=0; #50;
    A=0; B=1; C=1; D=1; #50;
    A=1; B=0; C=0; D=0; #50;
    A=1; B=0; C=0; D=1; #50;
    A=1; B=0; C=1; D=0; #50;
    A=1; B=0; C=1; D=1; #50;
    A=1; B=1; C=0; D=0; #50;
    A=1; B=1; C=0; D=1; #50;
    A=1; B=1; C=1; D=0; #50;
    A=1; B=1; C=1; D=1; #50;
    end
endmodule

module part5_1_test();
    reg A; reg B; reg C; wire Z;
    part5_1 p(A, B, C, Z);
    initial begin
            A=0; B=0; C=0; #100;
            A=0; B=0; C=1; #100;
            A=0; B=1; C=0; #100;  
            A=0; B=1; C=1; #100;
            A=1; B=0; C=0; #100;
            A=1; B=0; C=1; #100;
            A=1; B=1; C=0; #100;  
            A=1; B=1; C=1; #100;
            end
endmodule

module part5_2_test();
    reg A; reg B; reg C; wire Z;
    part5_2 p(A, B, C, Z);
    initial begin
            A=0; B=0; C=0; #100;
            A=0; B=0; C=1; #100;
            A=0; B=1; C=0; #100;  
            A=0; B=1; C=1; #100;
            A=1; B=0; C=0; #100;
            A=1; B=0; C=1; #100;
            A=1; B=1; C=0; #100;  
            A=1; B=1; C=1; #100;
            end
endmodule

module half_adder1_test();
    reg A, B; wire S, C; 
    half_adder1 ha(A, B, S, C);
    initial begin
        A = 0; B=0; #250;
        A=1; B=0; #250;
        A=0; B=1; #250;
        A=1; B=1; #250;
        end
endmodule

module full_adder1_test();
    reg A, B, Cin; wire S, Cout;
    full_adder1 fa(A, B, Cin, S, Cout);
    initial begin  
    A=0; B=0; Cin=0; #100;
    A=0; B=0; Cin=1; #100;
    A=0; B=1; Cin=0; #100;  
    A=0; B=1; Cin=1; #100;
    A=1; B=0; Cin=0; #100;
    A=1; B=0; Cin=1; #100;
    A=1; B=1; Cin=0; #100;  
    A=1; B=1; Cin=1; #100;
    end
endmodule

module full_adder4_test();
    reg [3:0] A, B;
    reg Cin;
    wire [3:0] S;
    wire Cout;
    full_adder4 fa4(A, B, Cin, S, Cout);
    initial begin
        A = 4'b1000; B = 4'b0001; Cin = 0;      #50;
        A= 4'b0010; B= 4'b0111; Cin = 0;      #50;
        A= 4'b0100; B= 4'b0101; Cin = 0;      #50;
        A= 4'b1011; B= 4'b1010; Cin = 0;      #50;
        A = 4'b1110; B= 4'b0101; Cin = 0;      #50;
        A = 4'b1111; B= 4'b1001; Cin = 0;      #50;
        A = 4'b0110; B= 4'b0011; Cin = 0;      #50;
        A = 4'b1000; B= 4'b1100; Cin = 0;      #50;
        end
endmodule

module full_adder8_test();
    reg [7:0] A, B;
    reg Cin;
    wire [7:0] S;
    wire Cout;
    full_adder8 fa8(A, B, Cin, S, Cout);
    initial begin
            A = 8'b00011101; B = 8'b00000101; Cin = 0;      #50;
            A = 8'b00110011; B = 8'b01011100;  Cin = 0;     #50;
            A = 8'b00010001; B = 8'b00011100; Cin = 0;      #50;
            A = 8'b10111111; B = 8'b00000010; Cin = 0;      #50;
            A = 8'b11001000; B = 8'b01011111; Cin = 0;      #50;
            A = 8'b00110001; B = 8'b00011001; Cin = 0;      #50;
            A = 8'b01001110; B = 8'b11111111; Cin = 0;      #50;
            A = 8'b00101011; B = 8'b00111011; Cin = 0;      #50;
            end
endmodule   

module full_adder16_test();
    reg[15:0] A, B;
    reg M;
    wire[15:0]S; wire Cout;
    full_adder16 fa16(A, B, M, S, Cout);
    initial begin
            A = 23;       B = 3;        M = 0;      #50;
            A = 21;       B = 75;       M = 0;      #50;
            A = 16800;    B = 16900;    M = 0;      #50;
            A = 6983;     B = 6650;     M = 0;      #50;
            A = 325;      B = 97;       M = 0;      #50;
            A = 44;       B = 190;      M = 0;      #50;
            A = 463;      B = 241;      M = 0;      #50;
            A = 86;       B = 572;      M = 0;      #50;
            end
endmodule    
    
    
module part11_test();
    reg [15:0] A, B;
    wire [15:0] S; wire Cout;
    part11 p11(A ,B, S, Cout);
    initial begin
            A = 32;       B = 7;             #50;
            A = 21;       B = 85;            #50;
            A = 16;       B = 36;            #50;
            A = 256;      B = 5;             #50;
            A = 200;      B = 95;            #50;
            A = 45;       B = 135;           #50;
            A = 36;       B = 255;           #50;
            A = 25;       B = 65;            #50;
            end

endmodule
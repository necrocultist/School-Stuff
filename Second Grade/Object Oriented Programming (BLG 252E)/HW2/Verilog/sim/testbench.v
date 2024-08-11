`timescale 1ns / 1ps


module SR_latch_test();
    reg S, R;
    wire Q, Q_neg;
    SR_latch UUT(S, R, Q, Q_neg);
    initial begin
        S = 0; R = 0; #125;
        S = 1; R = 0; #125;
        S = 0; R = 0; #125;
        S = 0; R = 1; #125;
        S = 0; R = 0; #125;
        S = 1; R = 0; #125;
        S = 1; R = 1; #125;
        S = 0; R = 1; #125;
        end
endmodule

module SR_latch_E_test();
    reg S, R, E;
    wire Q, Q_neg;
    SR_latch_E UUT(S, R, E, Q, Q_neg);
    initial begin
        E = 0; S = 0; R = 0; #125; 
        E = 1; S = 1; R = 0; #125; 
        E = 1; S = 0; R = 0; #125; 
        E = 0; S = 0; R = 1; #125; 
        E = 0; S = 0; R = 0; #125; 
        E = 1; S = 1; R = 0; #125; 
        E = 0; S = 1; R = 1; #125; 
        E = 1; S = 0; R = 1; #125; 
    end
endmodule

module SR_flipflop_test();
    reg S, R, CLK;
    wire Q, Q_neg;
    SR_flipflop ff(S, R, CLK, Q, Q_neg);
    initial begin
        CLK = 0;
        forever #100 CLK = ~CLK;
    end
    initial begin
        S = 0; R = 0; #200;
        S = 1; R = 0; #200;
        S = 0; R = 1; #200;
        S = 1; R = 1; #200;
        $finish;
    end
endmodule

module D_latch_test();
    reg D, E;
    wire Q, Q_neg;
    D_latch UUT(D, E, Q, Q_neg);
    initial begin
        D = 1; E = 0; #250;
        D = 1; E = 1; #250;
        D = 0; E = 0; #250;
        D = 0; E = 1; #250;
    end
endmodule

module NE_D_flipflop_test();
    reg D, CLK;
    wire Q, Q_neg;
    NE_D_flipflop UUT(D, CLK, Q, Q_neg);
    initial begin
        CLK = 0; D = 1; #200;
        D = 0; #200;
        D = 1; #200;
        $finish;
    end
    always begin
        CLK = ~CLK; #100;
      end
endmodule

module PE_D_flipflop_test();
    reg D, CLK;
    wire Q, Q_neg;
    PE_D_flipflop UUT(D, CLK, Q, Q_neg);
    
    initial begin
        CLK = 1; D = 1; #200;
        D = 0; #200;
        D = 1; #200;
        $finish;
    end
    always begin
        CLK = ~CLK; #100;
    end
endmodule

module PE_JK_flipflop_test();
    reg J, K, CLK;
    wire Q, Q_neg;
    PE_JK_flipflop UUT(J, K, CLK, Q, Q_neg);
    initial CLK = 0;
    always
          #125  CLK = ~CLK; 
    initial begin
        J = 0; K = 1; #250;
        J = 0; K = 0; #250;
        J = 1; K = 0; #250;
        J = 1; K = 1; #250;
    end
endmodule

module async_up_counter_test();
    reg J, K, CLK;
    wire[3:0] Q, Q_neg; 
    async_up_counter UUT(J, K, CLK, Q, Q_neg);
    
    initial begin
        CLK = 0;
        J = 0; K = 1;
        #30 
        J = 1; K = 1;
    end
    always #25 CLK = ~CLK;
endmodule

module sync_up_counter_test();
    reg J, K, CLK;
    wire[3:0] Q, Q_neg; 
    async_up_counter UUT(J, K, CLK, Q, Q_neg);
    
    initial begin
        CLK = 0;
        J = 0; K = 1;
        #30    
        J = 1; K = 1;
    end
    always #25 CLK = ~CLK;
endmodule

module pulse_generator_test();
    reg [15:0]in_data;
    reg load, CLK;
    wire Q;
    pulse_generator uut(in_data, load, CLK, Q);
    always #2.5 CLK = ~CLK;
    initial begin
        CLK = 0;
        in_data = 16'b1010101010101010;
    end
    
    initial begin
        load = 1;
        in_data = 16'b1010101010101010; #6;
        load = 0; #119;
        load = 1;
        in_data = 16'b1100110011001100; #6;
        load = 0; #119;
        load = 1;
        in_data = 16'b1111000011110000; #6;
        load = 0; #119;
        load = 1;
        in_data = 16'b1000000010000000; #6;
        load = 0; #119;
        load = 1;
        in_data = 16'b1110000000000000; #6;
        load = 0; #119;
        load = 1;
        in_data = 16'b1111111111000000; #6;
        load = 0; #119;
        $finish;
    end
endmodule
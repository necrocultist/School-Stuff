`timescale 1ns / 1ps

module tsb_test();
    reg [7:0] in; reg enable;
    wire out;
    tsb ts(in, enable, out);
    initial begin
        in = 8'd15; enable = 0; #10;
        enable = 1; #10;
        in = 8'd34; enable = 0; #10;
        enable = 1; #10;
        in = 8'd56;
    $finish;
    end
endmodule

module part1_test();
    reg [7:0] data1, data2;
    reg select;
    wire [7:0] out;
    part1 p1(data1, data2, select, out);
    initial begin
       data1 = 0; select = 0; #10;
       data1 = 8'd10; select = 1; #10;
       data2 = 8'd20; select = 0; #10;
       select = 1; #10;
       $finish;
    end
endmodule

module part2_test();
    reg [7:0] data1, data2;
    reg select; 
    wire [7:0] out1, out2;
    part2 p2(data1, data2, select, out1, out2);
    initial begin
        data1 = 0; select = 1; data2 = 8'd31;#10;
        data1 = 8'd60; select = 0; #10;
        data2 = 8'd55; select = 0; #10;
        select = 1; #10;
        $finish;
    end
endmodule

module part3_test();
    reg [7:0] in;
    reg reset, l_select, read_e, write_e, clk;
    wire [7:0] out;
    part3 p3(in, reset, l_select, read_e, write_e, clk, out);
    always #25 clk = ~clk;
    initial begin
        clk = 0; in = 8'd21; reset = 1; l_select = 1; read_e = 0; write_e = 1; #50;
        read_e = 1; write_e = 0; #50;
        reset = 0; #50;
        in = 8'd31; reset = 1; read_e = 0; write_e = 1; #50;
        read_e = 1; write_e = 0; #50;
        $finish;
    end
endmodule

module part4_test();
    reg [7:0] in;
    reg [2:0] address;
    reg chip_s, reset, read_e, write_e, clk;
    wire [7:0] out;
    part4 p4(in, address, chip_s, reset, read_e, write_e, clk, out);
    always #25 clk = ~clk;
    initial begin
        clk = 0; in = 8'd31; chip_s = 1; address = 3'b000; reset = 1; read_e = 0; write_e = 1; #50;
        write_e = 0; read_e = 1; #50;
        address = 3'b111; #50;
        in = 8'd21; read_e = 0; write_e = 1; address = 3'b001; #50;
        write_e = 0; read_e = 1; #50;
        in = 8'd41; read_e = 0; write_e = 1; address = 3'b010; #50;
        write_e = 0; read_e = 1; #50;
        reset = 0; #50;
        in = 8'd51; read_e = 0; write_e = 1; address = 3'b011; #50;
        write_e = 0; read_e = 1; #50;
        in = 8'd11; read_e = 0; write_e = 1; address = 3'b100; #50;
        write_e = 0; read_e = 1; #50;
        in = 8'd61; read_e = 0; write_e = 1; address = 3'b101; #50;
        write_e = 0; read_e = 1; #50;
        in = 8'd18; read_e = 0; write_e = 1; address = 3'b110; #50;
        write_e = 0; read_e = 1; #50;
        in = 8'd13; read_e = 0; write_e = 1; address = 3'b111; #50;
        write_e = 0; read_e = 1; #50;
        $finish;
    end
endmodule

module part5_test();
    reg [7:0] in;
    reg [4:0] address;
    reg reset,  read_e, write_e, clk;
    wire [7:0] out;
    part5 p5(in, address, reset, read_e, write_e, clk, out);
    always #25 clk = ~clk;
    initial begin
        clk = 0; reset = 1; #50;
        reset = 0; #50;
        in = 8'd25; address = 5'd30; read_e = 0; write_e = 1; #50;
        in = 8'd15; address = 5'd20; read_e = 0; write_e = 1; #50;
        address = 5'd12; read_e = 1; write_e = 0; #50;
        in = 8'd18; address = 5'd10; read_e = 0; write_e = 1; #50;
        address = 5'd15; read_e = 1; write_e = 0; #50;
        address = 5'd30; read_e = 1; write_e = 0; #50;
        address = 5'd10; read_e = 1; write_e = 0; #50;
        $finish;
    end
endmodule

module part6_test();
    reg[31:0] in;
    reg[4:0] address;
    reg reset, read_e, write_e, clk;
    wire[31:0] out;
    part6 p6(in, address, reset, read_e, write_e, clk, out);
    always #25 clk = ~clk;
    initial begin
        clk = 0; in = 32'ha5e4f569; address = 0; reset = 1; read_e = 0; write_e = 1; #50;
        read_e = 1; write_e = 0; #50;
        in = 32'hf49a34fe; address = 5'b01001; read_e = 0; write_e = 1; #50;
        read_e = 1; write_e = 0; #50;
        reset = 0; #50;
        in = 32'h67fe43a1; address = 5'b10010; read_e = 0; write_e = 1; #50;
        read_e = 1; write_e = 0; #50;
        address = 5'b10101; #50;
        in = 32'h45ad301d; address = 5'b00110; read_e = 0; write_e = 1; #50;
        read_e = 1; write_e = 0; #50;
        $finish;
    end
endmodule
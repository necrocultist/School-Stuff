`timescale 1ns / 1ps

module tsb(
    input [7:0] in,
    input enable,
    output reg [7:0] out
    );
    always @(*) begin
        if(enable) out = in;
        else out = 8'bz;
    end
endmodule

module part1(
    input [7:0] data1,
    input [7:0] data2,
    input select,
    output [7:0] out
);
    tsb b1(data1, !select, out);
    tsb b2(data2, select, out);
endmodule

module part2(
    input [7:0] data1,
    input [7:0] data2,
    input select, 
    output [7:0] out1,
    output [7:0] out2
);
    wire [7:0]bus;
    part1 p1(data1, data2, select, bus);
    tsb b0(bus, !select, out1);
    tsb b1(bus, select, out2);
endmodule

module part3(
    input [7:0] in,
    input reset, 
    input l_select, 
    input read_e,
    input write_e,
    input clk,
    output reg [7:0] out
);
    reg [7:0] memory;
    always @(posedge clk)begin
        if(write_e & l_select) memory <= in;
    end
    always @(negedge reset) begin
        memory <= 8'd0;
    end
    always @(*) begin
        if(read_e & l_select) out <= memory;
        else out <= 8'bz;
    end
endmodule

module part4(
    input [7:0] in,
    input [2:0] address,
    input chip_s,
    input reset,
    input read_e,
    input write_e,
    input clk,
    output [7:0] out
);
    part3 line0(in, reset, (!address[2] & !address[1] & !address[0] & chip_s), read_e, write_e, clk, out);
    part3 line1(in, reset, (!address[2] & !address[1] & address[0] & chip_s), read_e, write_e, clk, out);
    part3 line2(in, reset, (!address[2] & address[1] & !address[0] & chip_s), read_e, write_e, clk, out);
    part3 line3(in, reset, (!address[2] & address[1] & address[0] & chip_s), read_e, write_e, clk, out);
    part3 line4(in, reset, (address[2] & !address[1] & !address[0] & chip_s), read_e, write_e, clk, out);
    part3 line5(in, reset, (address[2] & !address[1] & address[0] & chip_s), read_e, write_e, clk, out);
    part3 line6(in, reset, (address[2] & address[1] & !address[0] & chip_s), read_e, write_e, clk, out);
    part3 line7(in, reset, (address[2] & address[1] & address[0] & chip_s), read_e, write_e, clk, out);
endmodule

module part5(
    input [7:0] in,
    input [4:0] address,
    input reset, 
    input read_e,
    input write_e,
    input clk,
    output [7:0] out
);
    part4 mem0(in, address[2:0], (!address[4] & !address[3]), reset, read_e, write_e, clk, out);
    part4 mem1(in, address[2:0], (!address[4] & address[3]), reset, read_e, write_e, clk, out);
    part4 mem2(in, address[2:0], (address[4] & !address[3]), reset, read_e, write_e, clk, out);
    part4 mem3(in, address[2:0], (address[4] & address[3]), reset, read_e, write_e, clk, out);
endmodule

module part6(
    input[31:0] in,
    input[4:0] address,
    input reset,
    input read_e,
    input write_e,
    input clk,
    output[31:0] out
);
    part5 mem0(in[7:0], address, reset, read_e, write_e, clk, out[7:0]);
    part5 mem1(in[15:8], address, reset, read_e, write_e, clk, out[15:8]);
    part5 mem2(in[23:16], address, reset, read_e, write_e, clk, out[23:16]);
    part5 mem3(in[31:24], address, reset, read_e, write_e, clk, out[31:24]);
endmodule

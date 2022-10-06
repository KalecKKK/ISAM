`timescale 1ns/1ps
module test;

reg sys_rst_n;
reg sys_clk;
reg [31:0] cmd;
reg cmd_en;
wire [15:0] cmd_id;
wire [31:0] res;
wire res_en;

cpu u_cpu (
    .sys_rst_n(sys_rst_n),
    .sys_clk(sys_clk),
    .cmd(cmd),
    .cmd_en(cmd_en),
    .cmd_id(cmd_id),
    .res(res),
    .res_en(res_en)
);

reg [31:0] cmd_mem[63:0];

initial begin
    sys_rst_n = 1'b1;
    #5 sys_rst_n = 1'b0;
    #5 sys_rst_n = 1'b1;
    
    /*
    // start
    cmd_mem[0] = 32'h00000000;
    // r0 = 0
    cmd_mem[1] = 32'h00000002;
    cmd_mem[2] = 32'h00000003;
    // r1 = 0
    cmd_mem[3] = 32'h00000102;
    cmd_mem[4] = 32'h00000103;
    // r2 = 1
    cmd_mem[5] = 32'h00000202;
    cmd_mem[6] = 32'h00010203;
    // r3 = 100
    cmd_mem[7] = 32'h00000302;
    cmd_mem[8] = 32'h00640303;
    // r1 = r1 + r2
    cmd_mem[9] = 32'h0201010A;
    // r0 = r0 + r1
    cmd_mem[10] = 32'h0100000A;
    // if r1 == r3 then miss
    cmd_mem[11] = 32'h00010306;
    // jmp to cmd No.9
    cmd_mem[12] = 32'h00000901;
    // output r0
    cmd_mem[13] = 32'h00000012;
    // pause
    cmd_mem[14] = 32'h00001401;
    */
    
cmd_mem[0] = 32'h00000000;
cmd_mem[1] = 32'h000a0803;
cmd_mem[2] = 32'h00000802;
cmd_mem[3] = 32'h00000703;
cmd_mem[4] = 32'h00000702;
cmd_mem[5] = 32'h00010603;
cmd_mem[6] = 32'h00000602;
cmd_mem[7] = 32'h00000505;
cmd_mem[8] = 32'h00000405;
cmd_mem[9] = 32'h00000305;
cmd_mem[10] = 32'h00000205;
cmd_mem[11] = 32'h00000105;
cmd_mem[12] = 32'h00000005;
cmd_mem[13] = 32'h00001801;
cmd_mem[14] = 32'h00000305;
cmd_mem[15] = 32'h00001501;
cmd_mem[16] = 32'h0601020e;
cmd_mem[17] = 32'h00070206;
cmd_mem[18] = 32'h0003030a;
cmd_mem[19] = 32'h06000010;
cmd_mem[20] = 32'h06010111;
cmd_mem[21] = 32'h00070106;
cmd_mem[22] = 32'h00001001;
cmd_mem[23] = 32'h00001e01;
cmd_mem[24] = 32'h00060404;
cmd_mem[25] = 32'h00060504;
cmd_mem[26] = 32'h0605050a;
cmd_mem[27] = 32'h00040004;
cmd_mem[28] = 32'h00050104;
cmd_mem[29] = 32'h00000e01;
cmd_mem[30] = 32'h00030404;
cmd_mem[31] = 32'h00080506;
cmd_mem[32] = 32'h00001a01;
cmd_mem[33] = 32'h00000412;
    
    cmd_en = 1'b1;
end

always begin
    #10 sys_clk <= 1'b0;
    #10 sys_clk <= 1'b1;
end

always @(posedge sys_clk) begin
    cmd <= cmd_mem[cmd_id];
end

always @(posedge res_en) begin
    cmd_en <= 1'b0;
end

endmodule

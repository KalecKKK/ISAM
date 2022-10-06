module cpu (
	input             sys_rst_n,
	input             sys_clk,
	input      [31:0] cmd,
	input             cmd_en,
	output reg [15:0] cmd_id,
	output reg [31:0] res,
	output reg        res_en
);

reg [31:0] reg_mem[15:0];

always @(negedge sys_clk or negedge sys_rst_n) begin
	if(!sys_rst_n) begin
		cmd_id  <= 32'b0;
		res     <= 32'b0;
		res_en  <= 1'b0;
	end
	else if(cmd_en) begin
	    if(res_en) res_en <= 1'b0;
		case(cmd[7:0])
            //  cmd of cmd_index
			8'd0: begin // pass
				cmd_id <= cmd_id + 1'b1;
			end
			8'd1: begin // jmp
				cmd_id <= cmd[23:8];
			end
			// cmd of register number
			8'd2: begin // wh
				reg_mem[cmd[15:8]][31:16] <= cmd[31:16];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd3: begin // wl
				reg_mem[cmd[15:8]][15:0]  <= cmd[31:16];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd4: begin // mov
				reg_mem[cmd[15:8]] <= reg_mem[cmd[23:16]];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd5: begin // rst
				reg_mem[cmd[15:8]] <= 32'b0;
				cmd_id <= cmd_id + 1'b1;
			end
			// cmd of compare
			8'd6: begin // ifeq
				if(reg_mem[cmd[15:8]] == reg_mem[cmd[23:16]])
    				cmd_id <= cmd_id + 2'b10;
    			else cmd_id <= cmd_id + 1'b1;
			end
			8'd7: begin // ifnq
				if(reg_mem[cmd[15:8]] != reg_mem[cmd[23:16]])
    				cmd_id <= cmd_id + 2'b10;
    			else cmd_id <= cmd_id + 1'b1;
			end
			8'd8: begin // ifgq
				if(reg_mem[cmd[15:8]] >= reg_mem[cmd[23:16]])
    				cmd_id <= cmd_id + 2'b10;
    			else cmd_id <= cmd_id + 1'b1;
			end
			8'd9: begin // iflq
				if(reg_mem[cmd[15:8]] <= reg_mem[cmd[23:16]])
    				cmd_id <= cmd_id + 2'b10;
    			else cmd_id <= cmd_id + 1'b1;
			end
			// cmd of calc
			8'd10: begin // add
				reg_mem[cmd[15:8]] <= reg_mem[cmd[23:16]] + reg_mem[cmd[31:24]];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd11: begin // sub
				reg_mem[cmd[15:8]] <= reg_mem[cmd[23:16]] - reg_mem[cmd[31:24]];
				cmd_id <= cmd_id + 1'b1;
			end
			// cmd of bits clac
			8'd12: begin // not
				reg_mem[cmd[15:8]] <= ~reg_mem[cmd[23:16]];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd13: begin // or
				reg_mem[cmd[15:8]] <= reg_mem[cmd[23:16]] | reg_mem[cmd[31:24]];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd14: begin // and
				reg_mem[cmd[15:8]] <= reg_mem[cmd[23:16]] & reg_mem[cmd[31:24]];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd15: begin // xor
				reg_mem[cmd[15:8]] <= reg_mem[cmd[23:16]] ^ reg_mem[cmd[31:24]];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd16: begin // sl
				reg_mem[cmd[15:8]] <= reg_mem[cmd[23:16]] << reg_mem[cmd[31:24]];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd17: begin // sr
				reg_mem[cmd[15:8]] <= reg_mem[cmd[23:16]] >> reg_mem[cmd[31:24]];
				cmd_id <= cmd_id + 1'b1;
			end
			8'd18: begin // out
                res <= reg_mem[cmd[15:8]];
                res_en <= 1'b1;
				cmd_id <= cmd_id + 1'b1;
			end
			default: cmd_id <= cmd_id + 1'b1;
		endcase
	end
end

endmodule

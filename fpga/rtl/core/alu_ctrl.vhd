library IEEE;
use IEEE.std_logic_1164.all;

entity alu_ctrl is
  port(
    alu_op      : in  std_logic_vector(1 downto 0);
    func_code   : in  std_logic_vector(5 downto 0);
    alu_control : out std_logic_vector(3 downto 0)
    );
end alu_ctrl;

architecture env_alu_ctrl of alu_ctrl is
begin
  process_alu_ctrl : process(alu_op, func_code)
  begin
    case alu_op(1 downto 0) is
      when "00" => alu_control <= "0010";          --add
      when "01" => alu_control <= "0110";          --subs
      when "10" =>
        case func_code(5 downto 0) is
          when "100000" => alu_control <= "0010";  --add
          when "100010" => alu_control <= "0110";  --subs
          when "100100" => alu_control <= "0000";  --and
          when "100101" => alu_control <= "0001";  --or
          when "100111" => alu_control <= "1100";  --nor
          when "101010" => alu_control <= "0111";  --slt
          when others   => alu_control <= "1111";
        end case;
      when others => alu_control <= "1111";
    end case;
  end process process_alu_ctrl;
end env_alu_ctrl;

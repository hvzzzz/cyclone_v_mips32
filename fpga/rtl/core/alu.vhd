library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity alu is
  port(
    in_a        : in  std_logic_vector(31 downto 0);
    in_b        : in  std_logic_vector(31 downto 0);
    alu_control : in  std_logic_vector(3 downto 0);
    zero        : out std_logic;
    alu_out     : out std_logic_vector(31 downto 0)
    );
end alu;

architecture env_alu of alu is
  signal tmp_out : std_logic_vector(31 downto 0);
begin
  alu_out <= tmp_out;
  zero    <= '1' when (tmp_out = x"00000000") else '0';

  process_alu : process(in_a, in_b, alu_control)
  begin
    case alu_control(3 downto 0) is
      when "0000" => tmp_out <= in_a and in_b;
      when "0001" => tmp_out <= in_a or in_b;
      when "0010" => tmp_out <= std_logic_vector(signed(in_a) + signed(in_b));
      when "0110" => tmp_out <= std_logic_vector(signed(in_a) - signed(in_b));
      when "0111" =>
        if signed(in_a) < signed(in_b) then
          tmp_out <= x"00000001";
        else
          tmp_out <= x"00000000";
        end if;
      when "1100" => tmp_out <= not(in_a or in_b);
      when others => tmp_out <= (others => '0');
    end case;
  end process process_alu;
end env_alu;

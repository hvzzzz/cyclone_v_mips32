library IEEE;
use IEEE.std_logic_1164.all;

entity main_ctrl_tb is
end main_ctrl_tb;

architecture main_ctrl_tb_arch of main_ctrl_tb is
  component main_ctrl
    port(
      op_code   : in  std_logic_vector(5 downto 0);
      reg_dst   : out std_logic;
      alu_src   : out std_logic;
      mem_2_reg : out std_logic;
      reg_write : out std_logic;
      mem_read  : out std_logic;
      mem_write : out std_logic;
      branch    : out std_logic;
      alu_op    : out std_logic_vector(1 downto 0)
      );
  end component;

  signal op_code   : std_logic_vector (5 downto 0) := "000000";
  signal reg_dst   : std_logic;
  signal alu_src   : std_logic;
  signal mem_2_reg : std_logic;
  signal reg_write : std_logic;
  signal mem_read  : std_logic;
  signal mem_write : std_logic;
  signal branch    : std_logic;
  signal alu_op    : std_logic_vector(1 downto 0);

begin
  UUT : main_ctrl
    port map(
      op_code   => op_code,
      reg_dst   => reg_dst,
      alu_src   => alu_src,
      mem_2_reg => mem_2_reg,
      reg_write => reg_write,
      mem_read  => mem_read,
      mem_write => mem_write,
      branch    => branch,
      alu_op    => alu_op
      );

  op_code <= "100011" after 10 ns, "101011" after 20 ns, "000100" after 30 ns, "111111" after 40 ns, "000000" after 50 ns;
end main_ctrl_tb_arch;

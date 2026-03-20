# Load the Quartus flow package
load_package flow

# Open your specific project
project_open mips_32

post_message "=================================================="
post_message " Starting Full Compilation Flow..."
post_message "=================================================="

# Execute the full synthesis, fitter, assembler, and timing analysis flow
execute_flow -compile

project_close
post_message "Build Complete!"

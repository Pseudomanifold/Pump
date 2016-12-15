node: examples/analyse_data
node: examples/generate_data
node: examples/pass_data

pass_data.out.1 -> analyse_data.in.1
generate_data.out.1 -> pass_data.in.1

import os
import json
import sys
import regex

# Ensure correct usage
if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <source_file> <output_directory>")
    sys.exit(1)

input_file = sys.argv[1]
output_dir = sys.argv[2]

json_file = "./tools/import_class.json"

# Read the JSON file into a dictionary
try:
    with open(json_file, "r") as f:
        json_data = json.load(f)
except FileNotFoundError:
    print(f"Could not open {json_file}")
    sys.exit(1)

# Ensure output directory exists
os.makedirs(output_dir, exist_ok=True)

# Write the first line with the #line directive
output_file = os.path.join(output_dir, os.path.basename(input_file))
try:
    with open(output_file, "w") as out_fh:
        out_fh.write(f'#line 1 "{input_file}"\n')

        # Read the entire input file content
        with open(input_file, "r") as in_fh:
            # State machine to track whether we're inside a comment or string
            in_string = None
            in_single_line_comment = False
            in_multi_line_comment = False

            line_index = 0

            for line in in_fh:
                new_line = ""
                line_index += 1
                index = 0

                # Process the line character by character
                while index < len(line):
                    char = line[index]
                    next_char = line[index + 1] if index + 1 < len(line) else ''

                    # Handle being inside a string
                    if in_string:
                        if char == "\\" and index < len(line) - 1:
                            # Handle escaped character
                            new_line += char + next_char
                            index += 2
                            continue
                        if char == in_string:
                            in_string = None  # Closing the string
                        new_line += char
                        index += 1
                        continue

                    # Handle being inside a single-line comment
                    if in_single_line_comment:
                        if char == "\n":
                            in_single_line_comment = False  # End of the comment
                        new_line += char
                        index += 1
                        continue

                    # Handle being inside a multi-line comment
                    if in_multi_line_comment:
                        if char == "*" and next_char == "/":
                            in_multi_line_comment = False
                            new_line += char + next_char
                            index += 2
                            continue
                        new_line += char
                        index += 1
                        continue

                    # Detect the start of a string
                    if char in ('"', "'"):
                        in_string = char
                        new_line += char
                        index += 1
                        continue

                    # Detect the start of a single-line comment
                    if char == "/" and next_char == "/":
                        in_single_line_comment = True
                        new_line += char + next_char
                        index += 2
                        continue

                    # Detect the start of a multi-line comment
                    if char == "/" and next_char == "*":
                        in_multi_line_comment = True
                        new_line += char + next_char
                        index += 2
                        continue

                    # Handle regex replacement outside of strings or comments
                    if not in_string and not in_single_line_comment and not in_multi_line_comment:
                        remaining_line = line[index:]
                        match = regex.match(r"^([a-zA-Z_][\da-zA-Z_]*||\([^)]*\))::\(([^)]*)\)([a-zA-Z_][\da-zA-Z_]*)(\((?:[^()]+|(?3))*+\))", remaining_line)
                        if match:
                            class_name, ret_type, method_name, attribute = match.groups()
                            attribute = attribute[1:-1] # Remove parentheses
                            new_line += f"{json_data["type_cast"]["".join(ret_type.split())]}(METHOD({method_name},{class_name}{"," if attribute else ""}{attribute}))"
                            index += len(match.group(0))
                            continue
                        match = regex.match(r"^([a-zA-Z_][\da-zA-Z_]*||\([^)]*\))::([^:]*)::([a-zA-Z_][\da-zA-Z_]*)(\((?:[^()]+|(?3))*+\))", remaining_line)
                        if match:
                            class_name, class_type, method_name, attribute = match.groups()
                            attribute = attribute[1:-1] # Remove parentheses
                            new_line += f"call_method_0(METHOD_TYPE({class_type},{method_name},{class_name}{"," if attribute else ""}{attribute}))"
                            index += len(match.group(0))
                            continue
                        match = regex.match(r"^([a-zA-Z_][\da-zA-Z_]*||\([^)]*\))::([a-zA-Z_][\da-zA-Z_]*)(\((?:[^()]+|(?3))*+\))", remaining_line)
                        if match:
                            class_name, method_name, attribute = match.groups()
                            attribute = attribute[1:-1] # Remove parentheses
                            if class_name in json_data["type_associations"]:
                                new_line += f"{json_data["type_caller"][class_name][json_data["method_index"][method_name]]}(METHOD_TYPE({json_data["type_associations"][class_name]},{method_name}{"," if attribute else ""}{attribute}))"
                            else:
                                new_line += f"call_method_0(METHOD({method_name},{class_name}{"," if attribute else ""}{attribute}))"
                            index += len(match.group(0))
                            continue
                        match = regex.match(r"^([a-zA-Z_][\da-zA-Z_]*||\([^)]*\)) *= *new +([a-zA-Z_][\da-zA-Z_]*)\(([^)]*)\)", remaining_line)
                        if match:
                            class_name, class_type, attribute = match.groups()
                            if class_type in json_data["type_associations"]:
                                new_line += f"{json_data["type_caller"][class_type][json_data["method_index"]["constructor"]]}(METHOD_TYPE({json_data["type_associations"][class_type]},constructor,{class_name}{"," if attribute else ""}{attribute}))"
                            else:
                                print(f"{input_file}:{line_index}:{index}: error: ‘{class_type}’ isn't a class name")
                            index += len(match.group(0))
                            continue

                    # Add the current character to the new line
                    new_line += char
                    index += 1

                # Write the modified line to the output file
                out_fh.write(new_line)

except FileNotFoundError:
    print(f"Cannot open input file {input_file}")
    sys.exit(1)

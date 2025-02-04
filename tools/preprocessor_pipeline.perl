#!/usr/bin/perl
use strict;
use warnings;
use JSON;

# Ensure correct usage
if (@ARGV != 2) {
    die "Usage: $0 <source_file> <output_directory>\n";
}

my $input_file = $ARGV[0];
my $output_dir = $ARGV[1];

my $json_file = "./tools/import_class.json";
my $json_text = do {
    open my $fh, "<", $json_file or die "Could not open $json_file: $!";
    local $/;
    <$fh>
};
my $json_data = decode_json($json_text);

# Ensure output directory exists
mkdir $output_dir unless -d $output_dir;

# Write the first line with the #line directive
my $output_file = "$output_dir/$input_file";
open my $out_fh, '>', $output_file or die "Cannot open output file $output_file: $!\n";
print $out_fh "#line 1 \"$input_file\"\n";

# Read the entire input file content
open my $in_fh, '<', $input_file or die "Cannot open input file $input_file: $!\n";

# State machine to track whether we're inside a comment or string
my $in_string = 0;
my $in_single_line_comment = 0;
my $in_multi_line_comment = 0;

while (<$in_fh>) {
    my $line = $_;
    my $new_line = "";
    my $index = 0;

    # Process the line character by character
    while ($index < length($line)) {
        my $char = substr($line, $index, 1);
        my $next_char = substr($line, $index + 1, 1) // '';

        # Handle being inside a string
        if ($in_string) {
            if ($char eq "\\" && $index < length($line) - 1) {
                # Handle escaped character
                $new_line .= $char . $next_char;
                $index += 2; # Skip the next character
                next;
            }
            if ($char eq $in_string) {
                $in_string = 0; # Closing the string
            }
            $new_line .= $char;
            $index++;
            next;
        }

        # Handle being inside a single-line comment
        if ($in_single_line_comment) {
            if ($char eq "\n") {
                $in_single_line_comment = 0; # End of the comment
            }
            $new_line .= $char;
            $index++;
            next;
        }

        # Handle being inside a multi-line comment
        if ($in_multi_line_comment) {
            if ($char eq '*' && $next_char eq '/') {
                $in_multi_line_comment = 0;
                $new_line .= $char . $next_char;
                $index += 2; # Skip the next character
                next;
            }
            $new_line .= $char;
            $index++;
            next;
        }

        # Detect the start of a string
        if ($char eq '"' || $char eq "'") {
            $in_string = $char;
            $new_line .= $char;
            $index++;
            next;
        }

        # Detect the start of a single-line comment
        if ($char eq '/' && $next_char eq '/') {
            $in_single_line_comment = 1;
            $new_line .= $char . $next_char;
            $index += 2; # Skip the next character
            next;
        }

        # Detect the start of a multi-line comment
        if ($char eq '/' && $next_char eq '*') {
            $in_multi_line_comment = 1;
            $new_line .= $char . $next_char;
            $index += 2; # Skip the next character
            next;
        }

        # Now handle the regex replacement outside of strings or comments
        # Create the substring for regex matching
        if (!$in_string && !$in_single_line_comment && !$in_multi_line_comment) {
            my $remaining_line = substr($line, $index);

            if ($remaining_line =~ /^([a-zA-Z_]+)::([a-zA-Z_]+)\(([^\)]*)\)/) {
                if (exists $json_data->{$1}) {
                    $new_line .= "METHOD_TYPE(". $json_data->{$1} . "," . $2 . "," . $3 . ")";
                    $index += length($1 . $2 . $3 . "(..)");
                    next;
                } else {
                    $new_line .= "METHOD(" . $2 . "," . $1 . "," . $3 . ")";
                    $index += length($1 . $2 . $3 . "(..)");
                    next;
                }
            }
        }

        # Add the current character to the new line
        $new_line .= $char;
        $index++;
    }

    # Print the modified line to the output file
    print $out_fh $new_line;
}


close $in_fh;


# Append the processed content to the output file
close $out_fh;
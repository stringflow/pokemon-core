require "fileutils"

module Docs
  def parse_exports(lines)
    lines = filter_lines(lines)

    lines.filter_map.each_with_index do |signature, index|
      next unless signature =~ %r{^DLLEXPORT}

      # gather signature, may be on multiple lines
      after = index
      signature << " " << lines[after += 1] until signature =~ %r{\{$}
      type, name, args = parse_signature(signature)

      # gather description from preceding comments
      before = index
      before -= 1 while before > 0 && lines[before - 1] =~ %r{^//}
      description = format_description(lines[before...index])

      [name, args, type, description]
    end
  end

  def filter_lines(lines)
    # remove leading/trailing whitespace
    # remove (resulting) empty lines
    lines.filter_map do |line|
      line.strip!
      line unless line.empty?
    end
  end

  def parse_signature(signature)
    match = signature.match(/^DLLEXPORT (?<type>.+) (?<name>.+)\((?<args>.*)\)\s*{$/)
    args = match[:args].split(/,\s*/)
    [match[:type], match[:name], args]
  end

  def format_description(lines)
    # remove comments' slashes and leading/trailing whitespace
    # drop special comments and following lines
    # remove linebreaks when: no period -> not uppercase
    lines.map { |line| line[2..].strip }
      .take_while { |line| line !~ /^[A-Z]+\(.+\):/ }
      .join("  \n")
      .gsub(/([^.])  \n([^A-Z])/, '\1 \2')
  end

  def format_section_link(name)
    "[#{name}](##{name})"
  end

  def build_toc_markdown(exports)
    toc = exports.map { |name, args, type, description| "- " + format_section_link(name) }
    toc.sort!
  end

  def convert_links(description, exports)
    function_names = exports.collect(&:first)
    function_names.each { |function_name| description.gsub!(function_name, format_section_link(function_name)) }
    description
  end

  def build_functions_markdown(exports)
    exports.flat_map do |name, args, type, description|
      [
        "### #{name}",
        "`(#{args.join(", ")}) -> #{type}`  ",
        convert_links(description, exports)
      ]
    end
  end
end

include Docs

FileUtils.mkdir_p("docs")

Dir["src/*.cpp"].each do |path|
  exports = parse_exports(File.readlines(path))
  next if exports.empty?

  markdown = [
    "# #{File.basename(path)}",
    "## Table of Contents",
    build_toc_markdown(exports),
    "## Functions",
    build_functions_markdown(exports)
  ]

  File.write("docs/#{File.basename(path, ".cpp")}.md", "#{markdown.join("\n")}\n")
end

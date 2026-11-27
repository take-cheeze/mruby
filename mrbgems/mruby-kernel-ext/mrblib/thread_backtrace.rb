class Thread
  module Backtrace
    # One frame of Kernel#caller_locations, as CRuby's Thread::Backtrace::Location: path, lineno, label and to_s
    class Location
      attr_reader :path, :lineno, :label

      def initialize(path, lineno, label)
        @path = path
        @lineno = lineno
        @label = label
      end

      # the path as the code was compiled, which is what path is; the absolute path is not known here
      def absolute_path
        raise NotImplementedError, "Thread::Backtrace::Location#absolute_path is not supported: only the path the code was compiled with is known"
      end

      def to_s
        @label ? "#{@path}:#{@lineno}:in #{@label}" : "#{@path}:#{@lineno}"
      end

      def inspect
        to_s.inspect
      end
    end
  end
end

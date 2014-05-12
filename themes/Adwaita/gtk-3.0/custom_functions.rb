module Sass::Script::Functions
    def gtkalpha(*args)
      return Sass::Script::String.new("alpha(#{args[0]},#{args[1]})")
    end
end

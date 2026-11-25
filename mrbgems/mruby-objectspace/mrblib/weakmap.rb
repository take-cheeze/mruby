# ObjectSpace::WeakMap, without weak references.
#
# This fork has no weak references, so the map holds its keys and values
# strongly and never drops an entry on its own. Keys are compared by identity,
# as in CRuby. Code that relies on entries disappearing after GC will keep
# objects alive here.
module ObjectSpace
  class WeakMap
    include Enumerable

    def initialize
      @map = {}.compare_by_identity
    end

    def []=(key, value)
      @map[key] = value
    end

    def [](key)
      @map[key]
    end

    def key?(key)
      @map.key?(key)
    end
    alias include? key?
    alias member? key?

    def delete(key)
      @map.delete(key)
    end

    def size
      @map.size
    end
    alias length size

    def keys
      @map.keys
    end

    def values
      @map.values
    end

    def each_pair(&block)
      @map.each_pair(&block)
      self
    end
    alias each each_pair

    def each_key(&block)
      @map.each_key(&block)
      self
    end

    def each_value(&block)
      @map.each_value(&block)
      self
    end
  end
end

template<typename Item>
class Iterator {
public:
  virtual Item & next() = 0;
  virtual bool hasNext() = 0;
};

template<typename Item>
class Collection {
  const int _max_elems;
  Item * _items;
  int _nElems;
public:
  Collection(int max_elems) : 
    _max_elems(max_elems), _nElems(0)
  {
    _items = new Item[_max_elems];
  }
  
  ~Collection() {
    delete[] _items;
  }
  
  
  inline Item & operator[](int i) {
    return _items[i];
  }
  
  inline Item & get(int i) {
    return _items[i];
  }
  
  inline void set(int i, Item & item) {
    _items[i] = item;
  }
  
  inline int nElems() { return _nElems; }

  bool add(Item item) {
    if(_nElems < _max_elems) {
      _items[_nElems] = item; // copy
      _nElems++;
      return true;
    } else {
      return false;
    }
  }
  
  class CollectionIterator : public Iterator<Item> {
    int _index = 0;
    Collection & _collection;
  public:
    CollectionIterator(Collection & collection):
      _collection(collection) {}

    Item & next() {
      if(_index < _collection.nElems())
        _index++;
      return _collection[_index-1];
    }
    
    bool hasNext() {
      return _index < _collection.nElems();
    }
  };
  
  Iterator<Item> * iterator() {
    return new CollectionIterator(*this);
  }
  
  class CiclicalCollectionIterator : public Iterator<Item> {
    int _index = 0;
    Collection & _collection;
  public:
    CiclicalCollectionIterator(Collection & collection):
      _collection(collection) {}
    
    Item & next() {
      if(_index < _collection.nElems())
        _index++;
      else
        _index = 1;
      
      return _collection[_index-1];
    }
    
    bool hasNext() {
      return true;
    }
  };
  
  Iterator<Item> * ciclicalIterator() {
    return new CiclicalCollectionIterator(*this);
  }
};


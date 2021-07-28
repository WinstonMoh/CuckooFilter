# Cuckoo Filter 🐦
I was going through an article by Martin Kleppmann on [Using Bloom filters to efficiently synchronize hash graphs](https://martin.kleppmann.com/2020/12/02/bloom-filter-hash-graph-sync.html) and he mentioned the cuckooo filter and how it could be better than a regular bloom filter. This got me interested and I wanted to learn more about it and attempt to design the data structure in c++. </br>
A c++ implementation of the Cuckoo Filter algorithm as explained in the [Cuckoo Filter paper](https://www.cs.cmu.edu/~dga/papers/cuckoo-conext2014.pdf).

## Introduction
- The cuckoo filters are used to approximate set memberships. They support adding and removing items dynamically while achieving even higher performance than Bloom filters.
- For applications that store many items and target moderately low false positive rates, cuckoo filters have lower space overhead than space-optimized Bloom filters.
- We store only fingerprints in the hash table to prevent inserting items in their entirety (perhaps externally to the table). But here, we use <i>partial-key cuckoo hashing</i> to find an item's alternate location based only on its fingerprint.
- A `fingerprint` is a bit string derived from the item using a hash function- for each item inserted, instead of key-value pairs, A set membership query for item `x`, and returns true if an identical fingerprint is found.

## Algorithms
The basic unit of the cuckoo hash tables used in our filters is called an `entry`. Each entry stores a fingerprint. The hash table consists of an array of buckets, where a bucket can have multiple entries.
### 1. Insert
We utilize a method called `partial-key cuckoo hashing` to derive an item's alternate location based on its fingerprint. </br>
For an item `x`, our hashing scheme calculates the indexes of the two candidate buckets as follows:
```python
h_1(x) = hash(x)                                    ......eqn(1)
h_2(x) = h_1(x) ⊕ hash(x's fingerprint)
```
This equation guarantees that h<sub>1</sub>(x) can also be calculated from h<sub>2</sub>(x) and the fingerprint using the same formula. This means that to displace a key `i` whether if `i` is h<sub>1</sub>(x) or h<sub>2</sub>(x), we calculate its alternate bucket `j` from the current bucket `i` and the fingerprint stored in this bucket by:
```python
j = i ⊕ hash(fingerprint)
```
There is more information on fingerprints and how the exact hashing is done which you can explore by looking at the paper. The algorithm for inserting an item is as follows:
```python
f = fingerprint(x) 
i1 = hash(x) 
i2 = i1 ⊕ hash(f) 
if bucket[i1] or bucket[i2] has an empty entry then 
  add f to that bucket
  return Done;

# must relocate existing items
i = randomly pick i1 or i2
for n = 0; n < MaxNumKicks; n++ do 
  randomly select an entry e from bucket[i]
  swap f and the fingerprint stored in entry e
  i = i ⊕ hash(f)
  if bucket[i] has an empty entry then 
    add f to bucket[i]
    return Done;

# Hashtable is considered full
return Failure
```

### 2. Lookup
Given an item `x`, the algorithm first calculates `x's` fingerprint and two candidate buckets according to `eqn(1)`.  These two buckets are read: if any existing fingerprint in either bucket matches, the cuckoo filter returns true, otherwise the filter returns false. This guarantees that no false negatives occur as long as bucket overflow never occurs. </br>
The algorithm is as follows:
``` python
f = fingerprint(x)
i1 = hash(x)
i2 = i1 ⊕ hash(f)
if bucket[i1] or bucket[i2] has f then 
  return True
return False
```

### 3. Delete
Given an item `x`, the filter checks both candidate buckets for the item; if any fingerprint matches in any bucket, one copy of that matched fingerprint is removed from that bucket. </br>
Note that to delete an item `x` safely, it must have been inserted previously. Otherwise, deleting a non-inserted item might unintentionally remove a real, different item that happens to share the same fingerprint. </br>
The algorithm is as follows:
```python
f = fingerprint(x)
i1 = hash(x)
i2 = i1 ⊕ hash(f)
if bucket[i1] or bucket[i2] has f then 
  remove a copy of f from this bucket
  return True
return False
```

## Usage

## Conclusion

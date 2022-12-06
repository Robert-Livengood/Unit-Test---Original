#include "pch.h"
#include <iostream>

// the global test environment setup and tear down
class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        //  initialize random seed
        srand(time(nullptr));
    }

    // Override this to define how to tear down the environment.
    void TearDown() override {}
};

// create the test class to house shared data between tests
class CollectionTest : public ::testing::Test
{
protected:
    // create a smart point to hold our collection
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override
    { // create a new collection to be used in the test
        collection.reset(new std::vector<int>);
    }

    void TearDown() override
    { //  erase all elements in the collection, if any remain
        collection->clear();
        // free the pointer
        collection.reset(nullptr);
    }

    // helper function to add random values from 0 to 99 count times to the collection
    void add_entries(int count)
    {
        assert(count > 0);
        for (auto i = 0; i < count; ++i)
            collection->push_back(rand() % 100);
    }
};

// Use ASSERT when failure should terminate processing, such as the reason for the test case.
// Use EXPECT when failure should notify, but processing should continue

// Test that a collection is empty when created.
// Prior to calling this (and all other TEST_F defined methods),
//  CollectionTest::StartUp is called.
// Following this method (and all other TEST_F defined methods),
//  CollectionTest::TearDown is called
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    // is the collection created
    ASSERT_TRUE(collection);

    // if empty, the size must be 0
    ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());

    // if empty, the size must be 0
    ASSERT_EQ(collection->size(), 0);
}

// Test that will always fail to demonstrate functionality
// NOTE: this is a negative test
TEST_F(CollectionTest, AlwaysFail)
{
    FAIL();
}

// Test to verify adding a single value to an empty collection
TEST_F(CollectionTest, CanAddToEmptyVector)
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);

    add_entries(1);

    // is the collection still empty?
    EXPECT_FALSE(collection->empty());

    // if not empty, what must the size be? -> 1
    ASSERT_EQ(collection->size(), 1);
}

// Test to verify adding five values to collection
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
    // is collection empty?
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);
    
    add_entries(5);

    // is the collection still empty?
    EXPECT_FALSE(collection->empty());

    // if not empty, what must the size be? -> 5
    ASSERT_EQ(collection->size(), 5);
}

// Test to verify that max size is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, MaxSizeGreaterThanOrEqualToSize)
{
    // verify max size is greater than or equal to 0 entries
    ASSERT_TRUE(collection->max_size() >= 0);

    // verify max size is greater than or equal to 1 entries
    ASSERT_TRUE(collection->max_size() >= 1);

    // verify max size is greater than or equal to 5 entries
    ASSERT_TRUE(collection->max_size() >= 5);

    // verify max size is greater than or equal to 10 entries
    ASSERT_TRUE(collection->max_size() >= 10);
}

// Test to verify that capacity is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, CapacityGreaterThanOrEqualToSize)
{
    // capacity greater than or equal to 0 entries
    ASSERT_TRUE(collection->capacity() >= 0);

    // capacity greater than or equal to 1 entries
    add_entries(1);  // 1 total
    ASSERT_TRUE(collection->capacity() >= 1);

    // capacity greater than or equal to 5 entries
    add_entries(4);  // 5 total
    ASSERT_TRUE(collection->capacity() >= 5);

    // capacity greater than or equal to 10 entries
    add_entries(5);  // 10 total
    ASSERT_TRUE(collection->capacity() >= 10);
}

// Test to verify resizing increases the collection size
TEST_F(CollectionTest, ResizingIncreasesCollectionSize)
{
    // initialize collection
    add_entries(1);

    // get current size and save as initial size
    int initialSize = collection->size();

    // resize collection to 2
    collection->resize(2);

    // verify collection increased in size
    ASSERT_TRUE(collection->size() > initialSize);
}

// Test to verify resizing decreases the collection
TEST_F(CollectionTest, ResizingDecreasesCollectionSize)
{
    // initialize collection
    add_entries(10);

    // get current size
    int initialSize = collection->size();

    // resize collection to zero
    collection->resize(1);

    //verify that the collection is now smaller
    ASSERT_TRUE(collection->size() < initialSize);
}

// Test to verify resizing decreases the collection to zero
TEST_F(CollectionTest, ResizingToZeroEntries)
{
    // initialize collection
    add_entries(1);

    // resize collection to zero
    collection->resize(0);

    // verify that the collection now has zero entries
    ASSERT_EQ(collection->size(), 0);
}

// Test to verify that clear function erases the collection
TEST_F(CollectionTest, ClearErasesCollection)
{
    // initialize collection to 3 entries
    add_entries(3);

    // clear collection
    collection->clear();

    // verify that the collection now has zero entries
    ASSERT_EQ(collection->size(), 0);
}

// Test to verify erase(begin,end) erases the collection
TEST_F(CollectionTest, VerifyEraseFunction)
{
    // initialize the collection to 15 entries
    add_entries(15);

    // use erase function from beginning to end of the collection
    collection->erase(collection->begin(), collection->end());

    // verify collection has been erased
    ASSERT_TRUE(collection->size() == 0);
}

// Test to verify reserve increases the capacity but not the size of the collection
TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize)
{
    // initialize the collection with 10 entries
    add_entries(10);

    // retain the original capacity and size of collection
    int initialCapacity = collection->capacity();
    int initialSize = collection->size();

    // reserve 15 enries, increasing capacity to 15 but the size should remain the same
    collection->reserve(15);

    // verify that the size of the collection is the same as before
    EXPECT_TRUE(collection->size() == initialSize);
    
    // verify that the capacity of the collection has now increased
    EXPECT_TRUE(collection->capacity() > initialCapacity);
}

// Test to verify the std::out_of_range exception is thrown when calling at() with an index out of bounds
TEST_F(CollectionTest, ExceptionThrownWhenOutOfBounds)
{
    // initialize collection to 10 entries
    add_entries(10);

    // verify that there is an exception thrown when calling an element outside the scope of collection
    ASSERT_ANY_THROW(collection->at(11));
}

// Verify that pop_back removes one element from collection
TEST_F(CollectionTest, RemoveOneEntry)
{
    // initialize collection to 5 entries
    add_entries(5);

    // retain initial size of collection
    int initialSize = collection->size();

    // remove one entry
    collection->pop_back();

    // collection size should now be one less than the initial size.
    ASSERT_TRUE((initialSize - collection->size()) == 1);
}

// Create a second failed test by asserting that the collection size is the same after add_entries
// NOTE: This is a negative test
TEST_F(CollectionTest, CustomFailedTest)
{
    // initialize to 5 entries
    add_entries(5);
    EXPECT_TRUE(collection->size() == 5);

    // retain initial collection size
    int initialSize = collection->size();
    EXPECT_TRUE(initialSize == 5);

    // add one entry
    add_entries(1);

    //create failure by saying the size is the same
    EXPECT_TRUE(collection->size() == initialSize);
}
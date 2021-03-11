CPSC-355 || Assignment 1

------------------------

A simple search engine to calculate the frequency of a word in different documents

The frequency = The value of the word[i,j]/The sum of the row[i]

Storing the frequency in a result [] array to sort it.

------------------------

The sorting algorithm is Quick Sort (QS):

QS is considered relatively quick in comparison with traditional sorting algorithm like bubble sort.

QS is a divide and conquer strategy, where we try to split our problem to sub or mini ones and then
performing our logic.

------------------------

The Best Case Scenario is nlog(n); however, this case can not be reached easily unless the PIVOT(*) is the median of the array.


AS well as, the AVERAGE CASE is highly predictable, which n log(n) as well, but it may takes some more steps than the best scenario.
In this case the PIVOT (*) is not the median but still its value is smaller than some numbers of the array
and greater than other numbers in the array.

The worst case scenario is n*n = n squared. This case can be seen if the given array is already sorted.
Worst case rarely occurs in most real-world data.

On the other hand, Merge Sort (MS) is better than QS because MS just has an avg case,which is nlog(n)

------------------------

NOTE:

*PIVOT: is the value of one of the array elements, which we use to split the array into two different sets (or sub-arrays).
--> The optimal PIVOT is the median.
--> We pick the PIVOT randomly like first element of the array OR last element OR a random index.
--> In assignment1, I choose the pivot as a last element.

------------------------

Finally, I used some online sources to recap these informations such as:

https://en.wikipedia.org/wiki/Quicksort

https://www.youtube.com/watch?v=-qOVVRIZzao

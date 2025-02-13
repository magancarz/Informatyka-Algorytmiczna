import Data.Char (toLower)
import Data.Map (fromListWith, toList)
import Data.List (nub, sort, delete, genericLength)

-- recursion
fac :: Int -> Int
fac n
	| n <= 1    = 1
	| otherwise = n * fac (n-1)

maximum' :: (Ord a) => [a] -> a
maximum' [] = error "maximum of empty list"
maximum' [x] = x
maximum' (x:xs) = max x (maximum' xs)

replicate' :: (Num i, Ord i) => i -> a -> [a]
replicate' n x
	| n <= 0    = []
	| otherwise = x:replicate' (n-1) x

take' :: (Num i, Ord i) => i -> [a] -> [a]
take' n _
	 | n <= 0   = []
take' _ []     = []
take' n (x:xs) = x : take' (n-1) xs

-- check if a string is pangram
-- import Data.Char (toLower)
isPangram :: String -> Bool
isPangram text = all (`elem` fixedText) ['a'..'z']
  where fixedText = map toLower text

-- collatz
collatz :: Int -> Int
collatz 1 = 1
collatz n = 
	if (even n)
		then (n `div` 2)
		else (3 * n + 1)

collatzSequence :: Int -> [Int]
collatzSequence n = 
	if n == 1
		then [1]
		else [n] ++ collatzSequence (collatz n)

collatzLength :: Int -> Int
collatzLength n = length (collatzSequence n)

-- check leap year
leapYear :: Int -> Int
leapYear n 
	| n `mod` 400 == 0 = n
	| n `mod` 100 == 0 = 0
	| n `mod` 4  == 0  = n
	| otherwise        = 0

findLeapYears = map leapYear [1..400]

-- rna transcription
transcribeToRna :: Char -> Char
transcribeToRna c
	| c == 'G' = 'C'
	| c == 'C' = 'G'
	| c == 'T' = 'A'
	| c == 'A' = 'U'

rnaTranscription = map transcribeToRna "GTACTA"

countLetters :: String -> Char -> Int
countLetters str c = length $ filter (==c) str

-- import Data.Map (fromListWith, toList)
frequency :: (Ord a) => [a] -> [(a, Int)]
frequency xs = toList (fromListWith (+) [(x, 1) | x <- xs])

-- sum of multiples
-- import Data.List (nub)
sumOfMultiples :: [Int] -> Int -> Int
sumOfMultiples xs n = sum (nub [k | k <- [1..(n-1)], x <- xs, k `mod` x == 0])

--grains on chessboard
countGrainsOnChessboard :: Integer
countGrainsOnChessboard = sum (map (2 ^) [0..63])

--acronyms (words must start with uppercase letter)
acronym :: String -> String
acronym s = [c | c <- s, c `elem` ['A'..'Z']]

--anagram
--import Data.List (sort)
anagram :: String -> [String] -> [String]
anagram s ls = [x | x <- ls, sort x == sort s]

--find nth prime number
isPrime :: Int -> Bool
isPrime n = factors n == [1,n]
	where factors n = [x | x <- [1..n], n `mod` x == 0]

nthPrime :: Int -> Int
nthPrime n = last (take n [i | i <- [1..], isPrime i])

--perfect, abundant and deficient numbers
--abundant and deficient funcs are the same as perfectNum, just change Eq sign
-- perfectNum :: Int -> Bool
-- perfectNum n = sum [k | k <- [1..n-1], n `mod` k == 0] == n

--functional2022
--ex 16
euler :: Int -> Int
euler n = length [k | k <- [1..n], gcd k n == 1]

eulerSum :: Int -> Int
eulerSum n = sum [euler k | k <- [1..n], n `mod` k == 0]

--ex 17
pithagoras :: Int -> [(Int, Int, Int)]
pithagoras n = [(a, b, c) | a <- [1..n], b <- [1..n], c <- [1..n], a^2 == b^2 + c^2 && gcd b c == 1]

--ex 18
fib :: Int -> Int
fib 0 = 0
fib 1 = 1
fib n = fib (n-1) + fib (n-2)

fib' :: Int -> Int
fib' n
	| n == 0 = 0
	| n == 1 = 1
	| n >= 2 = fib' (n-1) + fib' (n-2)

--ex 19
choose :: Int -> Int -> Int
choose n k
	| k == 0 || k == n = 1
	| n > k = choose (n-1) (k-1) + choose (n-1) k
	| otherwise = error "k > n"

--ex 20
perfectNum :: Int -> Bool
perfectNum n = sum [k | k <- [1..n-1], n `mod` k == 0] == n
findPerfectNums n = [p | p <- [1..n], perfectNum p]

--ex 21
add' = \x -> \y -> x + y
mul' = \x -> \y -> x * y

--ex 26
fib'' :: Int -> (Int, Int)
fib'' n 
	| n <= 1 = (0,1)
	| otherwise = (b, a + b)
	where (a, b) = fib'' (n-1)

fib''' :: Int -> Int
fib''' n = snd (fib'' n)

--ex 27
mymap :: (a -> b) -> [a] -> [b]
mymap f xs = [f x | x <- xs]

testMyMap = mymap (^ 2) [1..10]

--ex 28
mysum :: [Int] -> Int
mysum [x] = x
mysum (x:xs) = x + mysum xs

myproduct :: [Int] -> Int
myproduct [x] = x
myproduct (x:xs) = x * myproduct xs

myfact :: Int -> Int
myfact 0 = 1
myfact 1 = 1
myfact n = n * myfact (n-1)

--ex 29
mynub' :: [Int] -> [Int] -> [Int]
mynub' [] acc = acc
mynub' (x:xs) acc =
	if x `elem` acc
		then mynub' xs acc
	else mynub' xs (acc ++ [x])
mynub :: [Int] -> [Int]
mynub xs = mynub' xs []

--ex 30
myinits' :: [Int] -> Int -> [[Int]]
myinits' xs 0 = [take 0 xs]
myinits' xs acc = myinits' xs (acc-1) ++ [take acc xs]
myinits :: [Int] -> [[Int]]
myinits xs = myinits' xs (length (xs))

--ex 31
mytails' :: [Int] -> Int -> [[Int]]
mytails' xs 0 = [drop 0 xs]
mytails' xs acc = mytails' xs (acc-1) ++ [drop acc xs]
mytails :: [Int] -> [[Int]]
mytails xs = mytails' xs (length xs)

--ex 32
subs :: [Int] -> [[Int]]
subs [] = [[]]
subs (x:xs) = map (x:) (subs xs) ++ subs xs
partitions xs = [(ys, zs) | ys <- subs xs, zs <- subs xs, xs == (ys ++ zs)]

--ex 33
nondec :: Ord(a) => [a] -> Bool
nondec [] = True
nondec [x] = True
nondec (x:y:xs) = 
	if x <= y
		then nondec ([y]++xs)
	else False

--ex 34
myzip :: [a] -> [b] -> [(a, b)]
myzip [] ys = []
myzip xs [] = []
myzip (x:xs) (y:ys) = [(x, y)] ++ myzip xs ys

--ex 35
permutations :: Eq a => [a] -> [[a]]
permutations [] = [[]]
permutations as = do a <- as
                     let l = delete a as
                     ls <- permutations l
                     return (a : ls)

--ex 37
quicksort [] = []
quicksort (p:xs) = (quicksort lesser) ++ [p] ++ (quicksort greater)
	where
		lesser = filter (< p) xs
		greater = filter (>= p) xs

--ex 38
mmap f = map (map f)
mmmap f = map (map (map f))

--ex 39
--sprawdź typy i przetestuj działanie funkcji sum, product, all, any

--ex 40
--przetestuj działanie funkcji foldl, foldr, ...

--ex 41
sumEven :: [Int] -> Int
sumEven xs = foldr (\x y -> if (even x) then y + 1 else y) 0 xs

--ex 42
nondec42 :: [Int] -> Bool
nondec42 [x] = True
nondec42 (x:y:xs) = if(x <= y) then nondec42 (y:xs) else False

nondec42'' [] = True
nondec42'' (x:xs) = if(fst(x) <= snd(x)) then nondec42'' xs else False

nondec42' :: [Int] -> Bool
nondec42' xs = nondec42'' (zip xs (tail xs))

--ex 43
--1.foldl (-) e xs = e - sum xs "(((10 - 1) - 2) - 3)" True
--2.foldr (-) e xs = e - sum xs "(3 - (2 - (1 - 10)))" False

--haskellokwium 2
--ex 1
average1 xs = realToFrac (foldl (+) 0 xs) / genericLength xs
variance1 xs = (foldl (\x y -> x + (y - avg) ^ 2) 0 xs) / genericLength xs
	where avg = average1 xs

ex2func1 xs = average1 (map variance1 xs)
ex2func2 xs = variance1 (map average1 xs)

--ex 2
aciag = [((-1) ** i) / i | i <- [1.0, 2.0 .. 10.0]]

lewy x ai = foldl (\a b -> a + ((fst b) * (x ^ (snd b)))) 0 (zip ai [1..])
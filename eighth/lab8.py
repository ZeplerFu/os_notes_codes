import random
import matplotlib.pyplot as plt


def fifo(pages, f):
    array = []
    n = len(pages)
    top, page_faults = 0, 0

    for i in range(n):
        if len(array) < f:
            if pages[i] not in array:
                array.append(pages[i])
                page_faults = page_faults + 1

        else:
            if pages[i] not in array:
                array[top] = pages[i]
                top = (top+1) % f
                page_faults = page_faults + 1

    return page_faults


def main():
    n = input("Please input length of page trace as n: ")
    k = input("Please input number of page as k: ")

    n = int(n)
    k = int(k)

    if n < 16 or n % 2 != 0:
        print("n is invalid")
        return -1

    if k < 8 or k % 2 != 0:
        print("k is invalid")
        return -1

    page = []
    for i in range(n):
        page.append(random.randint(0, k - 1))
    print(page)

    faults = []
    for f in range(4, k + 1):
        faults.append(fifo(page, f))

    x = list(range(4, k + 1))
    y = faults
    plt.plot(x, y, c='red')

    plt.title('Changes of Number of Faults')
    plt.tick_params(axis='both', which='major')
    plt.xlabel('Number of frames')
    plt.ylabel('Number of faults')

    plt.show()
    return faults


if __name__ == '__main__':
    main()

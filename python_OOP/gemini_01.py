class BankAccount:
    def __init__(self, owner, balance):
        self.owner = owner  # 公开属性
        self.__balance = balance  # 私有属性 (外部无法直接访问)

    def deposit(self, amount):
        if amount > 0:
            self.__balance += amount
            print(f"存入 {amount}，当前余额: {self.__balance}")
        else:
            print("存款金额必须大于0")

    def get_balance(self):
        return self.__balance


# 使用
account = BankAccount("小明", 1000)
account.deposit(500)

# print(account.__balance)  # 报错！外部不能直接访问私有属性
print(account.get_balance())  # 必须通过类内部的方法访问

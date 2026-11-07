_id = 0


# FIXME: Private class method instead?
def get_next_id():
    global _id
    _id += 1
    return _id

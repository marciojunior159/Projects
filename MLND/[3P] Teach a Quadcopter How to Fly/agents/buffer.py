from collections import namedtuple, deque
import random

class ReplayBuffer:
    """Fixed-size buffer to store experience tuples."""

    def __init__(self, buffer_size, batch_size):
        """Initialize a ReplayBuffer object.
        Params
        ======
            buffer_size: maximum size of buffer
            batch_size: size of each training batch
        """
        self.memory = deque(maxlen=buffer_size)  # internal memory (deque)
        self.batch_size = batch_size
        self.experience = namedtuple("Experience", field_names=["state", "action", "reward", "next_state", "done"])
        self.episode = 0

    def add(self, state, action, reward, next_state, done):
        """Add a new experience to memory."""
        e = self.experience(state, action, reward, next_state, done)
        self.memory.append(e)
        
        if done and self.episode % 50 == 0 and self.episode > 50:
            self.episode += 1
            self.remove_bad_experience()
        elif done:
            self.episode += 1
        
    def sample(self, batch_size=16):
        """Randomly sample a batch of experiences from memory."""
        return random.sample(self.memory, k=self.batch_size)
    
    def average(self):
        """Return the current average reward of internal memory."""
        total = 0
        for t in self.memory:
            total += t.reward
        return total/self.__len__()
    
    def remove_bad_experience(self):
        """Removes experience that doesn't look promising."""
        average = self.average()
        to_delete = []
        for i, t in enumerate(self.memory):
            if t.reward < average:
                to_delete.append(i)
        f = 0        
        for d in to_delete:
            del self.memory[d-f]
            f += 1

    def __len__(self):
        """Return the current size of internal memory."""
        return len(self.memory)
                
                
            
import abc
from typing import List, Dict
from acj.core.network import UrbanNetwork
from acj.data.io import SimplificationResult
from acj.core.semantics import resolve_semantics
from acj.evaluation.metrics import Metric

class BaseEvaluator(abc.ABC):
    def __init__(self, original_network: UrbanNetwork, metrics: List[Metric]):
        self.original_network = original_network
        self.metrics = metrics
        
    @abc.abstractmethod
    def simplify(self) -> SimplificationResult:
        pass
        
    def evaluate(self) -> Dict[str, float]:
        result_de_simplificacion = self.simplify()
        self.simplified_network = resolve_semantics(self.original_network, result_de_simplificacion)
        
        results = {}
        for metric in self.metrics:
            metric_name = metric.__class__.__name__
            if metric_name.endswith('Metric'):
                metric_name = metric_name[:-6]
            results[metric_name] = metric.compute(self.original_network, self.simplified_network)
            
        return results

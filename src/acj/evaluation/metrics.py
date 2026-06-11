import abc
from acj.core.network import UrbanNetwork

class Metric(abc.ABC):
    @abc.abstractmethod
    def compute(self, original: UrbanNetwork, simplified: UrbanNetwork) -> float:
        pass

class CompressionRatioMetric(Metric):
    def compute(self, original: UrbanNetwork, simplified: UrbanNetwork) -> float:
        if len(original.nodes_df) == 0:
            return 0.0
        return 1.0 - (len(simplified.nodes_df) / len(original.nodes_df))

class SemanticSpeedDistortionMetric(Metric):
    def compute(self, original: UrbanNetwork, simplified: UrbanNetwork) -> float:
        total_distortion = 0.0
        count = 0
        
        for new_id, old_ids in simplified.lineage_edges.items():
            if new_id not in simplified.edge_metadata:
                continue
                
            new_speed = simplified.edge_metadata[new_id].get('maxspeed')
            if new_speed is None:
                continue
                
            for old_id in old_ids:
                if old_id in original.edge_metadata:
                    old_speed = original.edge_metadata[old_id].get('maxspeed')
                    if old_speed is not None:
                        try:
                            if isinstance(new_speed, str):
                                new_s = float(new_speed.split(' | ')[0])
                            else:
                                new_s = float(new_speed)
                                
                            if isinstance(old_speed, str):
                                old_s = float(old_speed.split(' | ')[0])
                            else:
                                old_s = float(old_speed)
                                
                            total_distortion += abs(new_s - old_s)
                            count += 1
                        except (ValueError, TypeError):
                            pass
                            
        if count == 0:
            return 0.0
            
        return total_distortion / count

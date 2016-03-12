namespace QXGame_Silverlight3.AStar
{
    using System.Collections.Generic;
    public interface IPathFinder
    {
        List<PathFinderNode> FindPath(Point start, Point end);
        void FindPathStop();

        bool Diagonals { get; set; }

        HeuristicFormula Formula { get; set; }

        bool HeavyDiagonals { get; set; }

        int HeuristicEstimate { get; set; }

        int SearchLimit { get; set; }

        bool Stopped { get; }

        bool TieBreaker { get; set; }
    }
}

